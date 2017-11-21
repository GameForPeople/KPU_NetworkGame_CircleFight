#pragma comment(lib, "ws2_32")

#include "mainServer.h"

CRITICAL_SECTION ACCEPT_SECTION, SIGNUP_SECTION, SIGNIN_SECTION;
static std::vector<UserData> userData;	//쓰레드에서도 사용해여하는데 메모리가 크기 떄문에, 쓰레드 인자로 넘기기보다 전역으로 선언하는게 더 올바른 방법으로 보임.

#pragma region [THREAD FUNCTION]
DWORD WINAPI ProcessClient(LPVOID arg) {
	ThreadStruct *threadStruct = (ThreadStruct *)arg;
	SOCKET clientSock = threadStruct->sock;

	int retVal;
	SOCKADDR_IN clientAddr;
	int addrlen;


	//클라이언트 정보 획득!
	addrlen = sizeof(clientAddr);
	getpeername(clientSock, (SOCKADDR *)&clientAddr, &addrlen);
	
	int recvType{};
	int sendType{};

	while (7) {
		recvType = ReturnTypeNumber(clientSock);

#pragma region [ LoginScene ]
		if (recvType == 100) {
			DemandLoginStruct demandLogin;

			retVal = recv(clientSock, (char*)&demandLogin, sizeof(demandLogin), 0);
			ErrorFunction(retVal, 0);

			bool isLoginSuccess{ false };

			if (demandLogin.type == 1) {
				//Login Function
				for (auto &i : userData) {
					if (!strcmp(demandLogin.ID, i.GetID())) {
						if (demandLogin.PW == i.GetPW()) {
							EnterCriticalSection(&SIGNIN_SECTION);
							if (!i.GetIsLogin()) {
								i.SetIPAddress(clientAddr.sin_addr);
								isLoginSuccess = true;
							}
							EnterCriticalSection(&SIGNIN_SECTION);
						}
					}
				}
			}
			else if (demandLogin.type == 2) {
				//화원가입
				EnterCriticalSection(&SIGNUP_SECTION);
				userData.emplace_back(demandLogin.ID, demandLogin.PW);
				userData[userData.size() - 1].SetIPAddress(clientAddr.sin_addr);
				isLoginSuccess = true;
				LeaveCriticalSection(&SIGNUP_SECTION);
			}

			if (isLoginSuccess) {
				sendType = 102;
			}
			else if (!isLoginSuccess) {
				sendType = 101;
			}

			retVal = send(clientSock, (char*)&sendType, sizeof(sendType), 0);
			ErrorFunction(retVal, 1);

			if (sendType == 102) {
				PermitLoginStruct permitLogin;
				retVal = send(clientSock, (char*)&permitLogin, sizeof(permitLogin), 0);
				ErrorFunction(retVal, 1);
			}
		}
#pragma endregion

#pragma region [ Lobby Scene ]


#pragma endregion
	}

	closesocket(clientSock);

	return 0;
}
#pragma endregion

int main(int argc, char *argv[])
{
	InitializeCriticalSection(&ACCEPT_SECTION);
	InitializeCriticalSection(&SIGNUP_SECTION);
	InitializeCriticalSection(&SIGNIN_SECTION);

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

	std::cout << "--------------------------------------------------- " << std::endl;
	std::cout << "And When We Advance			 // GAME MAIN SERVER  " << std::endl;
	std::cout << "                                ver 0.2  171121     " << std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;
	std::cout << "   IP ADDRESS  : " << serverAddr.sin_addr.s_addr		<< std::endl;
	std::cout << "   PORT NUMBER : " << serverAddr.sin_port << std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;

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

	std::cout << "   -- UserData Load Complete! " << std::endl;


#pragma endregion

#pragma region [ Listen() ]
	retVal = listen(listenSock, SOMAXCONN);
	if (retVal == SOCKET_ERROR) err_quit("listen()");

	std::cout << "   -- On Listen Server Start!! " << std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;


#pragma endregion

#pragma region [Thread Acept]

	SOCKET clientSock;
	SOCKADDR_IN clientAddr;
	int addrLen;
	HANDLE hThread;


	while (7) {
		//accept
		addrLen = sizeof(clientAddr);
		clientSock = accept(listenSock, (SOCKADDR *)&clientAddr, &addrLen);
		if (clientSock == INVALID_SOCKET) { err_display("accept()"); break; }

		std::cout << "[ 새로운 클라이언트 접속 IP : " << inet_ntoa(clientAddr.sin_addr) << "  PORT : " << ntohs(clientAddr.sin_port) << std::endl;
		
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

	DeleteCriticalSection(&ACCEPT_SECTION);
	DeleteCriticalSection(&SIGNUP_SECTION);
	DeleteCriticalSection(&SIGNIN_SECTION);


	// closesocket()
	closesocket(listenSock);

	// 윈속 종료
	WSACleanup();

	return 0;
#pragma endregion
}