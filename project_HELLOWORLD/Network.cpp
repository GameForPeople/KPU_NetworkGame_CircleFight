#include "Network.h"

Network::Network()
{
	m_sceneName = SceneName::Title;
	m_sendType = 0;
	m_recvType = 0;

	retVal = 0;

	//InitializeCriticalSection(&SEND_SECTION);
	InitializeCriticalSection(&LOBBY_UPDATE_SECTION);

}

Network::~Network()
{
	//DeleteCriticalSection(&SEND_SECTION);
	DeleteCriticalSection(&LOBBY_UPDATE_SECTION);
}


void Network::NetworkThreadFunction() {
	std::cout << "hello World! i'm Thread " << std::endl;
	while (7) {
		if (m_sceneName == SceneName::Login) {
			if (m_sendType) {
				std::cout << "타입 100번 보내요" << std::endl;
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				if (m_sendType == DEMAND_LOGIN) {
				std::cout << "로그인 또는  회원가입 정보 보내요!" << std::endl;
				std::cout << "보내는 타입은  " << m_demandLogin->type << std::endl;
				std::cout << "보내는 ID는  " << m_demandLogin->ID << std::endl;
				std::cout << "보내는 PW는  " << m_demandLogin->PW<< std::endl;
				std::cout << "보내는 SIZE는 *m_demandLogin " << sizeof(*m_demandLogin) << std::endl;
				std::cout << "보내는 SIZE는 m_demandLogin " << sizeof(m_demandLogin) << std::endl;
				std::cout << "보내는 SIZE는 &m_demandLogin " << sizeof(&m_demandLogin) << std::endl;

				std::cout << "로그인 실패 또는 성공보냅니다.!" << std::endl;
				retVal = send(m_sock, (char*)m_demandLogin, sizeof(*m_demandLogin), 0);

				//retVal = send(m_sock, (char*)&m_demandLogin, sizeof(m_demandLogin), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				std::cout << "로그인 실패 또는 성공 보냈어요!" << std::endl;
				retVal = recv(m_sock, (char*)&m_recvType, sizeof(m_recvType), 0);
				if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

				m_sendType = 0;

				if (m_recvType == PERMIT_LOGIN) {
					m_permitLogin = new PermitLoginStruct;

					retVal = recv(m_sock, (char*)m_permitLogin, sizeof(*m_permitLogin), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					std::cout << "받은 승리 횟수는 " << m_permitLogin->winCount << std::endl;
					std::cout << "받은 패배 횟수는 " << m_permitLogin->loseCount << std::endl;
					std::cout << "받은 사이즈의 크기는 " << sizeof(*m_permitLogin) << std::endl;
				}
			}
			}
		}
		if (m_sceneName == SceneName::Lobby) {
			//EnterCriticalSection(&SEND_SECTION);
			_sleep(100);
			//std::cout << "server : "<< m_sendType << " ";
				
			if (m_sendType) {
				//LeaveCriticalSection(&SEND_SECTION);

				std::cout << "로비입니다. 요구할게요 서버님!" << std::endl;
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				if (m_sendType == DEMAND_CHAT) {
					//std::cout << "5" << std::endl;

					retVal = send(m_sock, (char*)m_demandChat, sizeof(*m_demandChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					//std::cout << "6" << std::endl;
					

					//std::cout << "7 : " << sizeof(*m_permitChat) << std::endl;
					m_permitChat = new PermitChatStruct;
					
					retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;
					
					//std::cout << m_permitChat->chat[0] << std::endl;
					//std::cout << m_permitChat->chat[1] << std::endl;
					//std::cout << m_permitChat->chat[2] << std::endl;
					//std::cout << m_permitChat->chat[3] << std::endl;
					//std::cout << m_permitChat->chat[4] << std::endl;


					//std::cout << "8" << std::endl;

					m_sendType = 0;
					m_recvType = 1;
				}
				else if (m_sendType == DEMAND_CREATEROOM) {
					retVal = recv(m_sock, (char*)&m_recvType, sizeof(m_recvType), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					if (m_recvType == PERMIT_CREATEROOM) {

						m_permitCreateRoom = new PermitCreateRoomStruct;

						retVal = recv(m_sock, (char*)m_permitCreateRoom, sizeof(*m_permitCreateRoom), 0);
						if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

						m_sendType = 0;
					}
				}
				else if (m_sendType == DEMAND_JOINROOM) {
					retVal = send(m_sock, (char*)m_demandJoinRoom, sizeof(m_demandJoinRoom), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					retVal = recv(m_sock, (char*)&m_recvType, sizeof(m_recvType), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					if (m_recvType == PERMIT_JOINROOM) {
						m_permitJoinRoom = new PermitJoinRoomStruct;

						retVal = recv(m_sock, (char*)m_permitJoinRoom, sizeof(m_permitJoinRoom), 0);
						if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

						m_sendType = 0;
					}
				}
			}
			else {
				EnterCriticalSection(&LOBBY_UPDATE_SECTION);
					m_sendType = UPDATE_LOBBY;

					retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					m_permitChat = new PermitChatStruct;

					retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					//std::cout << m_permitChat->chat[0] << std::endl;
					//std::cout << m_permitChat->chat[1] << std::endl;
					//std::cout << m_permitChat->chat[2] << std::endl;
					//std::cout << m_permitChat->chat[3] << std::endl;
					//std::cout << m_permitChat->chat[4] << std::endl;

					m_updateLobbyInfo = new UpdateLobbyInfoStruct;

					retVal = recv(m_sock, (char*)m_updateLobbyInfo, sizeof(*m_updateLobbyInfo), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					m_sendType = 0;
					m_recvType = 2;

				LeaveCriticalSection(&LOBBY_UPDATE_SECTION);
			}
		}
	}

END_CONNECT:
	closesocket(m_sock);
	return;
}

bool Network::Connect() {
#pragma region [// 윈속 초기화]
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
#pragma endregion

#pragma region [ socket() ]
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion

#pragma region [ connect() ]
	static SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serverAddr.sin_port = htons(SERVERPORT);
	int retVal = connect(m_sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit("bind()");
#pragma endregion
}

bool Network::ErrorFunction(int value, int type) //type = 0 recv, type = 1send
{
	if (type == 0) {
		if (value == SOCKET_ERROR) { err_display("recv()"); return false; } // 서버야 니가 이상해..ㅎ 그래도 클라 접속끊을거야
		else if (!value) return false;	// 상대방의 접속이 이상함. 그러니 클라야 사요나라
		else return true;	//정상적.
	}
	else if (type == 1) {
		if (value == SOCKET_ERROR) { err_display("send()"); return false; }	// 몰라하른 서버 이상해 이놈. 그래도 클라 접속끊을거야
		else if (!value) return false;	// 0...? 이런일 안일어남.	굿바이 클라
		else return true;	// 정상적.
	}
}

int  Network::ReturnTypeNumber() {
	int type, retVal;

	std::cout << "   ToDebug : 데이터를 받으려고합니다." << std::endl;
	retVal = recv(m_sock, (char*)&type, sizeof(type), 0);
	if (!ErrorFunction(retVal, 0))
		return 0;
	std::cout << "   ToDebug : 데이터를 전송받았습니다.." << std::endl;

	return type;
}

// 소켓 함수 오류 출력 후 종료
void Network::err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void Network::err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}