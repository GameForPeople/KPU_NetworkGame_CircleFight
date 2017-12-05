#pragma comment(lib, "ws2_32")

#include "mainServer.h"
#include "GameCommuniCationProtocol.h"

CRITICAL_SECTION ACCEPT_SECTION, 
SIGNUP_SECTION, 
SIGNIN_SECTION, 
CREATE_DESTROY_ROOM_SECTION, 
IN_OUT_ROOM_SECTION,
UserDataAccess_SECTION, 
CHAT_SECTION;
//---------- ����ȭ�� ���ؼ�
// ACCEPT_SECTION :
// SIGNUP_SECTION : ȸ�����Ժκп���, ���ÿ� ȸ�������� �ϴ� ��츦 ����
// SIGNIN_SECTION : �α��� �κп���, ���ÿ� �ϳ��� ������ �α����� ��츦 ���� ���� ����
// UserDataAccess_Section : �α���, ȸ������, �α� �ƿ��� ���������Ϳ� �����ϴ� ��� ��쿡 ������ ����!

// ���� �� �ִ� ������ : SignUp���� ��Ȳ����, SignIn�� �䱸�� ���, ������ ���� ���ΰ�? --> ���ͷ����� ������ ����
// 
static std::vector<UserData> userData;	//�����忡���� �����di�ϴµ� �޸𸮰� ũ�� ������, ������ ���ڷ� �ѱ�⺸�� �������� �����ϴ°� �� �ùٸ� ������� ����.
static LobbyInfo lobbyData;// ��ü �κ� ����Ÿ.
static PermitChatStruct permitChatData;

//static bool ChatFlag{ false };
static bool IsSaveOn{ false };

#pragma region [THREAD FUNCTION]

DWORD WINAPI SaveUserDate(LPVOID arg) {
	while (7) {
		_sleep(10000);

		if (IsSaveOn) {
			std::ofstream outFile("UserData.txt", std::ios::out);
			char ID[5];
			int PW, winCount, loseCount;

			outFile << userData.size() << std::endl;

			for (auto i : userData) {
				outFile << " " << i.GetID()
					<< " " << i.GetPW()
					<< " " << i.GetWinCount()
					<< " " << i.GetLoseCount()
					<< std::endl;
				//outFile << ID << PW << winCount << loseCount;
			}
			outFile.close();

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

	//Ŭ���̾�Ʈ ���� ȹ��!
	addrlen = sizeof(clientAddr);
	getpeername(clientSock, (SOCKADDR *)&clientAddr, &addrlen);

	char ID[5];
	int PW;

	int recvType{};
	int sendType{};

	while (7) {
		recvType = ReturnTypeNumber(clientSock);	// Ŭ���̾�Ʈ�κ��� Ÿ�԰��� �޽��ϴ�.
		if (!recvType) goto END_CONNECT;

#pragma region [ LoginScene ]
		if (recvType == DEMAND_LOGIN) {
			DemandLoginStruct demandLogin{};

			retVal = recv(clientSock, (char*)&demandLogin, sizeof(demandLogin), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

			//std::cout << "�α��� �Ǵ�  ȸ������ ���� �޾Ҿ��!" << std::endl;
			//std::cout << "���� Ÿ����  " << demandLogin.type << std::endl;
			//std::cout << "���� ID��  " << demandLogin.ID << std::endl;
			//std::cout << "���� PW��  " << demandLogin.PW << std::endl;
			//std::cout << "���� �������  " << sizeof(demandLogin) << std::endl;

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
				//ȭ������
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
				std::cout << "[ Ŭ���̾�Ʈ �α��� ����  ���� ID��  " << demandLogin.ID << " ���� PW��  " << demandLogin.PW <<" IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;
				sendType = PERMIT_LOGIN;
				memcpy(ID, demandLogin.ID, sizeof(demandLogin.ID));
				PW = demandLogin.PW;
			}
			else if (!isLoginSuccess) {
				std::cout << "[ Ŭ���̾�Ʈ �α��� ����  ���� ID��  " << demandLogin.ID << " ���� PW��  " << demandLogin.PW << " IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;
				sendType = FAIL_LOGIN;
			}

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0); // �α��� ���� or ����, ������ ��� ����ϴ� �� �ʿ�!
			
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_LOGIN) {
				//�α��� ���������� �κ����� �����ּ���!
				PermitLoginStruct permitLogin;
				permitLogin.winCount = winBuffer;
				permitLogin.loseCount = loseBuffer;

				retVal = send(clientSock, (char*)&permitLogin, sizeof(permitLogin), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
				std::cout << "[ Ŭ���̾�Ʈ �κ� ����  ID��  " << demandLogin.ID << " IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;

			}
		}
#pragma endregion

#pragma region [ Lobby Scene ]
		if (recvType == DEMAND_CREATEROOM) {
			int roomIndexBuffer{};

			EnterCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
			if (roomIndexBuffer = lobbyData.CreateRoom(clientAddr.sin_addr)) {
				sendType = PERMIT_CREATEROOM;
			}
			else {
				sendType = FAIL_CREATEROOM;
				std::cout << "[ ����� ���� ID : " << ID << " IP : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
			}
			LeaveCriticalSection(&CREATE_DESTROY_ROOM_SECTION);

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0); 
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_CREATEROOM) {
				PermitCreateRoomStruct permitCreateRoom;
				permitCreateRoom.roomIndex = roomIndexBuffer;
				
				retVal = send(clientSock, (char*)&permitCreateRoom, sizeof(permitCreateRoom), 0); 
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				std::cout << "[ ����� ���� ID : " << ID  << "RoomIndex : " << roomIndexBuffer << " IP : " << inet_ntoa(clientAddr.sin_addr) << std::endl;
			}
		}
		else if (recvType == DEMAND_EXITROOM) {
			DemandExitRoomStruct demandExitRoom;
			retVal = recv(clientSock, (char*)&demandExitRoom, sizeof(demandExitRoom), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

			EnterCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
			if (lobbyData.ExitRoom(demandExitRoom.roomIndex)) {
				PermitExitRoomStruct permitExitRoom;
				retVal = send(clientSock, (char*)&permitExitRoom, sizeof(permitExitRoom), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
			}
			LeaveCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
		}
		else if (recvType == DEMAND_JOINROOM) {
			DemandJoinRoomStruct demandJoinRoom;
			retVal = recv(clientSock, (char*)&demandJoinRoom, sizeof(demandJoinRoom), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

			IN_ADDR hostAddrBuffer{};
			EnterCriticalSection(&IN_OUT_ROOM_SECTION);
			if(lobbyData.JoinRoom(demandJoinRoom.roomIndex, hostAddrBuffer)){
				sendType = PERMIT_JOINROOM;
			}
			else {
				sendType = FAIL_JOINROOM;
			}
			LeaveCriticalSection(&IN_OUT_ROOM_SECTION);

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0);
			if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

			if (sendType == PERMIT_JOINROOM) {
				PermitJoinRoomStruct permitJoinRoom;
				permitJoinRoom.hostAddr = hostAddrBuffer;

				retVal = send(clientSock, (char*)&permitJoinRoom, sizeof(permitJoinRoom), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
			}
		}
		else if (recvType == DEMAND_CHAT) {
			DemandChatStruct demandChat;
			retVal = recv(clientSock, (char*)&demandChat, sizeof(demandChat), 0);
			if (!ErrorFunction(retVal, 0)) goto END_CONNECT;
			
			lobbyData.PushChat();

			char buf[CHAT_BUF_SIZE]{};
			
			//std::cout << "���� ä�� ������ : " << demandChat.chat << std::endl;
			memcpy(buf, ID, sizeof(ID));
			strcat(buf, " : ");
			strcat(buf, demandChat.chat);
			//std::cout << "��ȯ�� ä�� ������ : " << buf << std::endl;

			//strncpy(lobbyData.m_chatBuf[4], buf, sizeof(CHAT_BUF_SIZE));
			for (int i = 0; i < CHAT_BUF_SIZE; i++)
				lobbyData.m_chatBuf[4][i] = buf[i];


			std::cout << "����� ä�� ������ : " << lobbyData.m_chatBuf[4] << std::endl;

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

			//for (int i = 0; i < CHAT_MAX_LINE; i++) {
			//	for (int j = 0; j < CHAT_BUF_SIZE; j++) {
			//		permitChatData.chat[i][j] = lobbyData.m_chatBuf[i][j];
			//	}
			//	//strncpy(permitChatData.chat[i], (lobbyData.m_chatBuf[i]), sizeof(CHAT_BUF_SIZE));
			//	//std::cout << permitChatData.chat[i] << std::endl;
			//}

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

#pragma endregion

#pragma region [ inGame Scene ]

#pragma endregion

	}

END_CONNECT:
	std::cout << "[ Ŭ���̾�Ʈ ���� ID : " << ID  << " Ŭ���̾�Ʈ ���� IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] " << std::endl;

	EnterCriticalSection(&UserDataAccess_SECTION);
	for (auto &i : userData) {
		if (!strcmp(ID, i.GetID())) {
			if(i.GetIsLogin()) {
				i.SetIsLogin(false);
			}
		}
	}
	LeaveCriticalSection(&UserDataAccess_SECTION);
	
	closesocket(clientSock);
	return 0;
}
#pragma endregion


int main(int argc, char *argv[])
{
	int retVal;

#pragma region [// ���� �ʱ�ȭ]
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

	std::cout << "�������������������������������" << std::endl;
	std::cout << "��  And When We Advance	             // GAME MAIN SERVER  ��" << std::endl;
	std::cout << "��                                       ver 2.8  171129  ��" << std::endl;
	std::cout << "��                                                        ��" << std::endl;
	std::cout << "��  IP ADDRESS  : " << /*serverAddr.sin_addr.s_addr*/ ip << "                         ��"<< std::endl;
	std::cout << "��  PORT NUMBER : " << SERVERPORT << "                                    ��"<< std::endl;
	std::cout << "�������������������������������" << std::endl << std::endl;

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

	InitializeCriticalSection(&ACCEPT_SECTION);
	InitializeCriticalSection(&SIGNUP_SECTION);
	InitializeCriticalSection(&SIGNIN_SECTION);
	InitializeCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
	InitializeCriticalSection(&IN_OUT_ROOM_SECTION);
	InitializeCriticalSection(&UserDataAccess_SECTION);

	std::cout << "	- Init Critical Section Complete! " << std::endl;

	//lobbyData �ʱ�ȭ �ʿ�

	for ( int i = 0 ; i < CHAT_MAX_LINE ; i++)
		permitChatData.chat[i][0] = '\0';

	std::cout << "	- Init Static Structure Complete! " << std::endl;

#pragma endregion

#pragma region [ Listen() ]
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR) err_quit("listen()");

	std::cout << "	- On Listen!,   Server Start! " << std::endl << std::endl;


#pragma endregion

#pragma region [Thread Acept]

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	HANDLE hThread, hThreadSaveData;

	hThread = CreateThread(NULL, 0, SaveUserDate, NULL , 0, NULL);
	

	while (7) {
		//accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET) { err_display("accept()"); break; }

		std::cout << "[ ���ο� Ŭ���̾�Ʈ ���� IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << "  ] "<< std::endl;
		
		ThreadStruct threadArgument;
		threadArgument.sock = clientSock;

		hThread = CreateThread(NULL, 0, ProcessClient, &threadArgument, 0, NULL);
		
		if (hThread == NULL) { closesocket(clientSock); }
		else { CloseHandle(hThread); }
	}

#pragma endregion

#pragma region [ GoodBye Server! ]
	std::cout << "---Master- ������ �����Ͻðڽ��ϱ�? --> ";
	char buf{};
	std::cin >> buf;

	DeleteCriticalSection(&ACCEPT_SECTION);
	DeleteCriticalSection(&SIGNUP_SECTION);
	DeleteCriticalSection(&SIGNIN_SECTION);
	DeleteCriticalSection(&CREATE_DESTROY_ROOM_SECTION);
	DeleteCriticalSection(&IN_OUT_ROOM_SECTION);
	DeleteCriticalSection(&UserDataAccess_SECTION);

	// closesocket()
	closesocket(listenSock);

	// ���� ����
	WSACleanup();

	return 0;
#pragma endregion
}

