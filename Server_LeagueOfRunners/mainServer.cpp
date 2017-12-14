#pragma comment(lib, "ws2_32")

#include "mainServer.h"
#include "GameCommuniCationProtocol.h"

CRITICAL_SECTION 
		//ACCEPT_SECTION, 
		//SIGNUP_SECTION, 
		//SIGNIN_SECTION, 
		CREATE_DESTROY_ROOM_SECTION, 
		IN_OUT_ROOM_SECTION,
		UserDataAccess_SECTION, 
		CHAT_SECTION;

//---------- 동기화에 관해서
// ACCEPT_SECTION : 액셒 받을때!! --> 사요나라
// SIGNUP_SECTION : 회원가입부분에서, 동시에 회원가입을 하는 경우를 제한 --> 바이
// SIGNIN_SECTION : 로그인 부분에서, 동시에 하나의 계정에 로그인할 경우를 막기 위해 생성 -->바이
// UserDataAccess_Section : 로그인, 회원가입, 로그 아웃등 유저데이터에 접근하는 모든 경우에 제한을 두자!
// CHAT_SECTION : 채팅에 한번에 여러가지가 못들어가게하자!
// 생길 수 있는 문제점 : SignUp중인 상황에서, SignIn이 요구될 경우, 문제가 없을 것인가? --> 이터레이터 사이즈 문제
//--------------------------

static std::vector<UserData> userData;	//쓰레드에서도 사용해di하는데 메모리가 크기 떄문에, 쓰레드 인자로 넘기기보다 전역으로 선언하는게 더 올바른 방법으로 보임.
static LobbyInfo lobbyData;// 전체 로비 데이타.
static PermitChatStruct permitChatData;

static bool IsSaveOn{ false };
static std::atomic<int> playersNumber;

#pragma region [THREAD FUNCTION]

DWORD WINAPI SaveUserDate(LPVOID arg) {
	while (7) {
		Sleep(1000);

		if (IsSaveOn) {
			Sleep(2000);
			std::ofstream outFile("UserData.txt", std::ios::out);
			char ID[MAX_ID_LEN];
			int PW, winCount, loseCount;

			outFile << userData.size() << std::endl;

			for (auto i : userData) {
				outFile << " " << i.GetID()
					<< " " << i.GetPW()
					<< " " << i.GetWinCount()
					<< " " << i.GetLoseCount()
					<< std::endl;
			}
			outFile.close();
			std::cout << "[ System - UserDataSave ]" << std::endl;

			IsSaveOn = false;
		}
	}
	return 0;
}

DWORD WINAPI ProcessClient(LPVOID arg) {
	ThreadStruct *threadStruct = (ThreadStruct *)arg;
	SOCKET clientSock = threadStruct->sock;

	int retVal;
	SOCKADDR_IN clientAddr;
	int addrlen;

	//클라이언트 정보 획득!
	addrlen = sizeof(clientAddr);
	getpeername(clientSock, (SOCKADDR *)&clientAddr, &addrlen);

	char ID[MAX_ID_LEN];
	int PW;

	int recvType{};
	int sendType{};
	int roomIndex = -1;

	SceneName nowScene = SceneName::Login;

	while (7) {
		recvType = ReturnTypeNumber(clientSock);	// 클라이언트로부터 타입값을 받습니다.
		if (!recvType) goto END_CONNECT;

		std::cout << "받은 번호는 : " << recvType << std::endl;

#pragma region [ LoginScene ]
		if (recvType == DEMAND_LOGIN) {
			DemandLoginStruct demandLogin{};

			retVal = recv(clientSock, (char*)&demandLogin, sizeof(demandLogin), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

		#ifdef DEBUG_MODE
			//std::cout << "로그인 또는  회원가입 정보 받았어요!" << std::endl;
			//std::cout << "받은 타입은  " << demandLogin.type << std::endl;
			//std::cout << "받은 ID는  " << demandLogin.ID << std::endl;
			//std::cout << "받은 PW는  " << demandLogin.PW << std::endl;
			//std::cout << "받은 사이즈는  " << sizeof(demandLogin) << std::endl;
		#endif

			bool isLoginSuccess{ false };
			int winBuffer, loseBuffer{};

			if (demandLogin.type == 1) {
				//Login Function
							EnterCriticalSection(&UserDataAccess_SECTION);
				for (auto &i : userData) {
					if (!strcmp(demandLogin.ID, i.GetID())) {
						if (demandLogin.PW == i.GetPW()) {
							if (!i.GetIsLogin()) {
								i.SetIPAddress(clientAddr.sin_addr);
								isLoginSuccess = true;
								memcpy(ID,demandLogin.ID, sizeof(demandLogin.ID));
								PW = demandLogin.PW;
								winBuffer = i.GetWinCount();
								loseBuffer = i.GetLoseCount();
							}
						}
					}
				}
							LeaveCriticalSection(&UserDataAccess_SECTION);
			}
			else if (demandLogin.type == 2) {
				//화원가입
				bool isFail = false;

				EnterCriticalSection(&UserDataAccess_SECTION);
				for (auto &i : userData) {
					if (!strcmp(demandLogin.ID, i.GetID())) {
						isFail = true;
						isLoginSuccess = false;
					}
				}

				if (!isFail) {
					userData.emplace_back(demandLogin.ID, demandLogin.PW);
					userData[userData.size() - 1].SetIPAddress(clientAddr.sin_addr);
					isLoginSuccess = true;
					memcpy(ID, demandLogin.ID, sizeof(demandLogin.ID));
					PW = demandLogin.PW;
				}

					LeaveCriticalSection(&UserDataAccess_SECTION);
					winBuffer = 0;
					loseBuffer = 0;
					IsSaveOn = true;
			}

			if (isLoginSuccess) {

				#ifdef DEBUG_MODE
				std::cout << "[ 클라이언트 로그인 성공  받은 ID는  " << demandLogin.ID << " 받은 PW는  " << demandLogin.PW <<" IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;
				#endif		

				sendType = PERMIT_LOGIN;
				memcpy(ID, demandLogin.ID, sizeof(demandLogin.ID));
				PW = demandLogin.PW;
				nowScene = SceneName::Lobby;
				playersNumber++;
			}
			else if (!isLoginSuccess) {
				#ifdef DEBUG_MODE
				std::cout << "[ 클라이언트 로그인 실패  받은 ID는  " << demandLogin.ID << " 받은 PW는  " << demandLogin.PW << " IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;
				#endif
				sendType = FAIL_LOGIN;
				//std::cout << sendType;
			}


			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0); // 로그인 성공 or 실패, 성공일 경우 대기하는 것 필요!
			std::cout << " a " << sendType;
			
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_LOGIN) {
				//로그인 성공했으니 로비데이터 보내주세요!
				PermitLoginStruct permitLogin;
				permitLogin.winCount = winBuffer;
				permitLogin.loseCount = loseBuffer;

				retVal = send(clientSock, (char*)&permitLogin, sizeof(permitLogin), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
				
				#ifdef DEBUG_MODE
				std::cout << "[ 클라이언트 로비 접속  ID는  " << demandLogin.ID << " IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;
				#endif
			}
		}
#pragma endregion

#pragma region [ Lobby Scene ]
		if (recvType == DEMAND_CREATEROOM) {
			int roomIndexBuffer{};

			EnterCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
			if ((roomIndexBuffer = lobbyData.CreateRoom(clientAddr.sin_addr)) >= 0) {
				sendType = PERMIT_CREATEROOM;
			}
			else {
				sendType = FAIL_CREATEROOM;
				#ifdef DEBUG_MODE
				std::cout << "[ 방생성 실패 ID : " << ID << " IP : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				#endif
			}
			LeaveCriticalSection(&CREATE_DESTROY_ROOM_SECTION);

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0); 
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_CREATEROOM) {
				PermitCreateRoomStruct permitCreateRoom;
				permitCreateRoom.roomIndex = roomIndexBuffer;
				roomIndex = roomIndexBuffer;

				retVal = send(clientSock, (char*)&permitCreateRoom, sizeof(permitCreateRoom), 0); 
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				nowScene = SceneName::Room;

				#ifdef DEBUG_MODE
				std::cout << "[ "<< roomIndexBuffer <<" 번 방생성 성공 ID : " << ID  << " IP : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				#endif
			}
		}
		else if (recvType == DEMAND_JOINROOM) {
			DemandJoinRoomStruct demandJoinRoom;
			retVal = recv(clientSock, (char*)&demandJoinRoom, sizeof(demandJoinRoom), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

			IN_ADDR hostAddrBuffer{};
			EnterCriticalSection(&IN_OUT_ROOM_SECTION);
			//EnterCriticalSection(&CREATE_DESTROY_ROOM_SECTION);

			if(lobbyData.JoinRoom(demandJoinRoom.roomIndex, hostAddrBuffer)){
				sendType = PERMIT_JOINROOM;
				roomIndex = demandJoinRoom.roomIndex;
			}
			else {
				sendType = FAIL_JOINROOM;
			}
			LeaveCriticalSection(&IN_OUT_ROOM_SECTION);
			//LeaveCriticalSection(&CREATE_DESTROY_ROOM_SECTION);

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0);
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_JOINROOM) {
				PermitJoinRoomStruct permitJoinRoom;
				permitJoinRoom.hostAddr = hostAddrBuffer;

				retVal = send(clientSock, (char*)&permitJoinRoom, sizeof(permitJoinRoom), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				nowScene = SceneName::RoomGuest;

				#ifdef DEBUG_MODE
				std::cout << "[ " << roomIndex << " 번 방 접속 성공 ID : " << ID << " IP : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
				#endif
			}
		}
		else if (recvType == DEMAND_CHAT) {
			DemandChatStruct demandChat;
			retVal = recv(clientSock, (char*)&demandChat, sizeof(demandChat), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;
			
			EnterCriticalSection(&CHAT_SECTION);
			lobbyData.PushChat();

			char buf[CHAT_BUF_SIZE]{};
			
			#ifdef DEBUG_MODE
			//std::cout << "받은 채팅 내용은 : " << demandChat.chat << std::endl;
			#endif

			memcpy(buf, ID, sizeof(ID));
			strcat(buf, " : ");
			strcat(buf, demandChat.chat);

			#ifdef DEBUG_MODE
			//std::cout << "변환된 채팅 내용은 : " << buf << std::endl;
			#endif

			//strncpy(lobbyData.m_chatBuf[4], buf, sizeof(CHAT_BUF_SIZE));
			for (int i = 0; i < CHAT_BUF_SIZE; i++)
				lobbyData.m_chatBuf[4][i] = buf[i];

			#ifdef DEBUG_MODE
			std::cout << "[ 채팅이 입력되었습니다.  : " << lobbyData.m_chatBuf[4] << " ] "<< std::endl;
			#endif

			LeaveCriticalSection(&CHAT_SECTION);

			// 채팅 보내는 로직으로 다시 바꿀경우 아래 모두 복귀!!

			//for (int i = 0; i < CHAT_MAX_LINE; i++) {
			//	for (int j = 0; j < CHAT_BUF_SIZE; j++) {
			//		permitChatData.chat[i][j] = lobbyData.m_chatBuf[i][j];
			//	}
			//	//strncpy(permitChatData.chat[i], (lobbyData.m_chatBuf[i]), sizeof(CHAT_BUF_SIZE));
			//	//std::cout << permitChatData.chat[i] << std::endl;
			//}

			//ChatFlag = true;
			//retVal = send(clientSock, (char*)&lobbyData.m_chatBuf, sizeof(permitChatData), 0);
			////retVal = send(clientSock, (char*)&permitChatData, sizeof(permitChatData), 0);
			//if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

		}
		else if (recvType == UPDATE_LOBBY) {

			retVal = send(clientSock, (char*)&lobbyData.m_chatBuf, sizeof(permitChatData), 0);
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			UpdateLobbyInfoStruct lobbyRoomBuffer;
			
			for (int i = 0; i < MAX_ROOM_COUNT; i++) {
				lobbyRoomBuffer.m_playersNumber[i] = lobbyData.GetRoomPlayersNumber(i);
				lobbyRoomBuffer.m_mapNumber[i] = lobbyData.GetRoomMapNumber(i);
			}

			retVal = send(clientSock, (char*)&lobbyRoomBuffer, sizeof(lobbyRoomBuffer), 0);
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
		}
#pragma endregion

#pragma region [ Room Scene ]
		if (recvType == DEMAND_EXITROOM) {

		//	std::cout << "1 ";
			EnterCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
			//std::cout << "2 ";
			EnterCriticalSection(&IN_OUT_ROOM_SECTION);
			//std::cout << "3 ";

			if (nowScene == SceneName::RoomGuest)
				lobbyData.ExitRoom(roomIndex);
			else if (nowScene == SceneName::Room)
				lobbyData.DestoryRoom(roomIndex);

			LeaveCriticalSection(&IN_OUT_ROOM_SECTION);
			LeaveCriticalSection(&CREATE_DESTROY_ROOM_SECTION);

			nowScene = SceneName::Lobby;
		}

#pragma endregion

#pragma region [ inGame Scene ]
		if (recvType == DEMAND_SENDRESULT) {

			int WinLoseBuf{};
			retVal = recv(clientSock, (char*)&WinLoseBuf, sizeof(WinLoseBuf), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

			for (auto &i : userData) {
				if (!strcmp(ID, i.GetID())) {
					i.SetWinOrLose(WinLoseBuf);
					//std::cout << "[승리 혹은 패배 적용 완료]" << std::endl;
					break;
				}
			}
			IsSaveOn = true;
		}

#pragma endregion
	
	}

END_CONNECT:
	std::cout << "[ 클라이언트 종료 ID : " << ID  << " 클라이언트 종료 IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;

	EnterCriticalSection(&UserDataAccess_SECTION);
	for (auto &i : userData) {
		if (!strcmp(ID, i.GetID())) {
			if(i.GetIsLogin()) {
				i.SetIsLogin(false);
			}
		}
	}
	LeaveCriticalSection(&UserDataAccess_SECTION);
	
	if (nowScene == SceneName::RoomGuest)
		lobbyData.ExitRoom(roomIndex);
	else if (nowScene == SceneName::Room)
		lobbyData.DestoryRoom(roomIndex);

	playersNumber--;

	closesocket(clientSock);
	return 0;
}
#pragma endregion

int main(int argc, char *argv[])
{
	int retVal;

#pragma region [// 윈속 초기화]
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
#pragma endregion

#pragma region [ socket() ]
	SOCKET listenSock = socket(AF_INET, SOCK_STREAM, 0);
	if (listenSock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion

#pragma region [ Bind() ]
	SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serverAddr.sin_port = htons(SERVERPORT);
	retVal = bind(listenSock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit("bind()");
#pragma endregion

#pragma region [Print ServerUI]

	PHOSTENT host;
	char name[255];
	char *ip;

	if (gethostname(name, sizeof(name)) == 0);
	if ((host = gethostbyname(name)) != NULL)
	{
		ip = inet_ntoa(*(struct in_addr *)*host->h_addr_list);

	}

	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << std::endl;
	std::cout << "■  And When We Advance	             // GAME MAIN SERVER  ■" << std::endl;
	std::cout << "■                                       ver 2.8  171129  ■" << std::endl;
	std::cout << "■                                                        ■" << std::endl;
	std::cout << "■  IP ADDRESS  : " << /*serverAddr.sin_addr.s_addr*/ ip << "                         ■"<< std::endl;
	std::cout << "■  PORT NUMBER : " << SERVERPORT << "                                    ■"<< std::endl;
	std::cout << "■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■" << std::endl << std::endl;

#pragma endregion

#pragma region [Load UserData]

	std::ifstream inFile("UserData.txt", std::ios::in);
	char ID[5];
	int PW, winCount, loseCount;

	int userDataCount{};

	inFile >> userDataCount;

	for (int i = 0; i < userDataCount; i++) {
		inFile >> ID >> PW >> winCount >> loseCount;

		userData.emplace_back(ID, PW, winCount, loseCount);
	}

	inFile.close();

	std::cout << "	- UserData Load Complete! " << std::endl;

#pragma endregion

#pragma region [ Init Part ]

	//InitializeCriticalSection(&ACCEPT_SECTION);
	//InitializeCriticalSection(&SIGNUP_SECTION);
	//InitializeCriticalSection(&SIGNIN_SECTION);
	InitializeCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
	InitializeCriticalSection(&IN_OUT_ROOM_SECTION);
	InitializeCriticalSection(&UserDataAccess_SECTION);
	InitializeCriticalSection(&CHAT_SECTION);

	std::cout << "	- Init Critical Section Complete! " << std::endl;

	//lobbyData 초기화 필요

	for ( int i = 0 ; i < CHAT_MAX_LINE ; i++)
		permitChatData.chat[i][0] = '\0';

	playersNumber = 0;

	std::cout << "	- Init Static Structure Complete! " << std::endl;

#pragma endregion

#pragma region [ Listen() ]
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR) err_quit("listen()");

	std::cout << "	- On Listen!,   Server Start! " << std::endl << std::endl;


#pragma endregion

#pragma region [ Thread Acept ]

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	HANDLE hThread, hThreadSaveData;

	hThreadSaveData = CreateThread(NULL, 0, SaveUserDate, NULL , 0, NULL);
	
	while (7) {
		//accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET) { err_display("accept()"); break; }

		std::cout << "[ 새로운 클라이언트 접속 IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] "<< std::endl;
		
		ThreadStruct threadArgument;
		threadArgument.sock = clientSock;

		hThread = CreateThread(NULL, 0, ProcessClient, &threadArgument, 0, NULL);
		
		if (hThread == NULL) { closesocket(clientSock); }
		else { CloseHandle(hThread); }
	}

#pragma endregion

#pragma region [ GoodBye Server! ]
	std::cout << "---Master- 서버를 종료하시겠습니까? --> ";
	char buf{};
	std::cin >> buf;

	//DeleteCriticalSection(&ACCEPT_SECTION);
	//DeleteCriticalSection(&SIGNUP_SECTION);
	//DeleteCriticalSection(&SIGNIN_SECTION);
	DeleteCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
	DeleteCriticalSection(&IN_OUT_ROOM_SECTION);
	DeleteCriticalSection(&UserDataAccess_SECTION);
	DeleteCriticalSection(&CHAT_SECTION);

	// closesocket()
	closesocket(listenSock);

	// 윈속 종료
	WSACleanup();

	return 0;
#pragma endregion
}

