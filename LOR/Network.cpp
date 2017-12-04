#include "Network.h"
// ����� ���� ������ ���� �Լ�
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) { return SOCKET_ERROR; }
		else if (received == 0) { break; }
		left -= received;
		ptr += received;
	}

	return (len - left);
}

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
				std::cout << "Ÿ�� 100�� ������" << std::endl;
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				if (m_sendType == DEMAND_LOGIN) {
				std::cout << "�α��� �Ǵ�  ȸ������ ���� ������!" << std::endl;
				std::cout << "������ Ÿ����  " << m_demandLogin->type << std::endl;
				std::cout << "������ ID��  " << m_demandLogin->ID << std::endl;
				std::cout << "������ PW��  " << m_demandLogin->PW<< std::endl;
				std::cout << "������ SIZE�� *m_demandLogin " << sizeof(*m_demandLogin) << std::endl;
				std::cout << "������ SIZE�� m_demandLogin " << sizeof(m_demandLogin) << std::endl;
				std::cout << "������ SIZE�� &m_demandLogin " << sizeof(&m_demandLogin) << std::endl;

				std::cout << "�α��� ���� �Ǵ� ���������ϴ�.!" << std::endl;
				retVal = send(m_sock, (char*)m_demandLogin, sizeof(*m_demandLogin), 0);

				//retVal = send(m_sock, (char*)&m_demandLogin, sizeof(m_demandLogin), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				std::cout << "�α��� ���� �Ǵ� ���� ���¾��!" << std::endl;
				retVal = recv(m_sock, (char*)&m_recvType, sizeof(m_recvType), 0);
				if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

				m_sendType = 0;

				if (m_recvType == PERMIT_LOGIN) {
					if(m_permitLogin == NULL)
						m_permitLogin = new PermitLoginStruct;

					retVal = recv(m_sock, (char*)m_permitLogin, sizeof(*m_permitLogin), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					std::cout << "���� �¸� Ƚ���� " << m_permitLogin->winCount << std::endl;
					std::cout << "���� �й� Ƚ���� " << m_permitLogin->loseCount << std::endl;
					std::cout << "���� �������� ũ��� " << sizeof(*m_permitLogin) << std::endl;
				}
			}
			}
		}
		if (m_sceneName == SceneName::Lobby) {
			//EnterCriticalSection(&SEND_SECTION);
			_sleep(200);
			//std::cout << "server : "<< m_sendType << " ";
				
			if (m_sendType > 0) {
				//LeaveCriticalSection(&SEND_SECTION);

				std::cout << "�κ��Դϴ�. �䱸�ҰԿ� ������!" << std::endl;
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
				
				std::cout << " " << m_sendType << std::endl;
				
				if (m_sendType == DEMAND_CHAT) {

					retVal = send(m_sock, (char*)m_demandChat, sizeof(*m_demandChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					if(m_permitChat == NULL)
						m_permitChat = new PermitChatStruct;
					
					retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;
					
					//std::cout << m_permitChat->chat[0] << std::endl;
					//std::cout << m_permitChat->chat[1] << std::endl;
					//std::cout << m_permitChat->chat[2] << std::endl;
					//std::cout << m_permitChat->chat[3] << std::endl;
					//std::cout << m_permitChat->chat[4] << std::endl;

					m_sendType = 0;
					m_recvType = 1;
				}
				else if (m_sendType == DEMAND_CREATEROOM) {
					retVal = recv(m_sock, (char*)&m_recvType, sizeof(m_recvType), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					if (m_recvType == PERMIT_CREATEROOM) {

						if(m_permitCreateRoom == NULL)
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
						if(m_permitJoinRoom == NULL)
							m_permitJoinRoom = new PermitJoinRoomStruct;

						retVal = recv(m_sock, (char*)m_permitJoinRoom, sizeof(m_permitJoinRoom), 0);
						if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

						m_sendType = 0;
					}
				}
			}
			else if(m_sendType == 0){
				EnterCriticalSection(&LOBBY_UPDATE_SECTION);
					m_sendType = UPDATE_LOBBY;

					retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					if(m_permitChat == NULL)
						m_permitChat = new PermitChatStruct;

					retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					//std::cout << m_permitChat->chat[0] << std::endl;
					//std::cout << m_permitChat->chat[1] << std::endl;
					//std::cout << m_permitChat->chat[2] << std::endl;
					//std::cout << m_permitChat->chat[3] << std::endl;
					//std::cout << m_permitChat->chat[4] << std::endl;

					if (m_updateLobbyInfo == NULL)
						m_updateLobbyInfo = new UpdateLobbyInfoStruct;

					retVal = recv(m_sock, (char*)m_updateLobbyInfo, sizeof(*m_updateLobbyInfo), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					m_sendType = -1;
					m_recvType = 2;

				LeaveCriticalSection(&LOBBY_UPDATE_SECTION);
			}
		}
		else _sleep(1000);
	}

END_CONNECT:
	closesocket(m_sock);
	return;
}

bool Network::Connect() {
#pragma region [// ���� �ʱ�ȭ]
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return false;
#pragma endregion

#pragma region [ socket() ]
	m_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion
	std::cout << " League Of Runners ������ �����մϴ�. " << std::endl;
	std::cout << " ������ IP�� �Է����ּ��� : ";
	char ipBuffer[20]{};
	ipBuffer[0] = '\0';
	std::cin >> ipBuffer;

#pragma region [ connect() ]
	static SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(ipBuffer);
	serverAddr.sin_port = htons(SERVERPORT);
	int retVal = connect(m_sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit("bind()");
#pragma endregion
}

bool Network::ErrorFunction(int value, int type) //type = 0 recv, type = 1send
{
	if (type == 0) {
		if (value == SOCKET_ERROR) { err_display("recv()"); return false; } // ������ �ϰ� �̻���..�� �׷��� Ŭ�� ���Ӳ����ž�
		else if (!value) return false;	// ������ ������ �̻���. �׷��� Ŭ��� ��䳪��
		else return true;	//������.
	}
	else if (type == 1) {
		if (value == SOCKET_ERROR) { err_display("send()"); return false; }	// �����ϸ� ���� �̻��� �̳�. �׷��� Ŭ�� ���Ӳ����ž�
		else if (!value) return false;	// 0...? �̷��� ���Ͼ.	�¹��� Ŭ��
		else return true;	// ������.
	}
}

int  Network::ReturnTypeNumber() {
	int type, retVal;

	std::cout << "   ToDebug : �����͸� ���������մϴ�." << std::endl;
	retVal = recv(m_sock, (char*)&type, sizeof(type), 0);
	if (!ErrorFunction(retVal, 0))
		return 0;
	std::cout << "   ToDebug : �����͸� ���۹޾ҽ��ϴ�.." << std::endl;

	return type;
}

// ���� �Լ� ���� ��� �� ����
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

// ���� �Լ� ���� ���
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


deque<QueueData> sendQueue[MAX_PLAYER];
deque<int> sendQueueGuest;
int m_idx;
UpdateFrameStruct basicInfo;
Pawn* charArr;
int	*emotionNum;
int  *emotionTime;

int numPlayer = 0;
UpdateRoomStruct roomInfo;
atomic<int> readyPlayer;
SOCKET listen_sock = NULL;

bool gameStart;
HANDLE hThreadGuest[2];
CharacterName reqChar;
deque<UseItemInfo> itemQueue;

int GetEmptySlot()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (roomInfo.m_charInfo[i] == CharacterName::NONE) return i;
	return -1;
}

DWORD WINAPI SendData(LPVOID arg)
{
	RoomInfoStruct sock_info = *(RoomInfoStruct*)arg;

	int retval;
	bool running = true;

	QueueData sendOp;

	while (running)
	{
		if (!sendQueue[sock_info.idx].empty())
		{
			sendOp = sendQueue[sock_info.idx].front();
			sendQueue[sock_info.idx].pop_front();
			send(sock_info.sock, (char*)&sendOp.op, sizeof(sendOp.op), 0);
			switch (sendOp.op)
			{
				// InRoom���
			case UPDATE_ROOM:
				send(sock_info.sock, (char*)&roomInfo, sizeof(roomInfo), 0);
				break;
			case NOTIFY_CHANGEMAP:
				send(sock_info.sock, (char*)&roomInfo.m_mapInfo, sizeof(roomInfo.m_mapInfo), 0);
				break;
			case PERMIT_CHANGECHAR:
			{
				PermitChangeCharStruct chaInfo(sendOp.fromIdx, roomInfo.m_charInfo[sendOp.fromIdx]);
				send(sock_info.sock, (char*)&chaInfo, sizeof(chaInfo), 0);
				break;
			}
			case PERMIT_EXIT:
				running = false;
				roomInfo.m_charInfo[sock_info.idx] = CharacterName::NONE;

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
					sendQueue[i].emplace_back(UPDATE_ROOM);
				}

				send(sock_info.sock, (char*)&running, sizeof(running), 0);
				closesocket(sock_info.sock);
				numPlayer--;
				break;
			case NOTIFY_EXIT:
				running = false;
				closesocket(sock_info.sock);
				break;

				// InGame ���
			case UPDATE_FRAME:
				send(sock_info.sock, (char*)&basicInfo, sizeof(basicInfo), 0);
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI RecvData(LPVOID arg)
{
	RoomInfoStruct sock_info = *(RoomInfoStruct*)arg;

	int retval, op;
	bool running = true;

	while (running)
	{
		retval = recvn(sock_info.sock, (char*)&op, sizeof(op), 0);
		if (retval > 0)
		{
			switch (op)
			{
			case -1:	// Ÿ�Ӿƿ�
				break;
				// InRoom ���
			case REQUEST_CHANGECHAR:
				retval = recvn(sock_info.sock, (char*)&roomInfo.m_charInfo[sock_info.idx], sizeof(roomInfo.m_charInfo[sock_info.idx]), 0);

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
					sendQueue[i].emplace_back(PERMIT_CHANGECHAR, sock_info.idx);
				}
				break;
			case REQUEST_EXIT:
				running = false;
				closesocket(sock_info.sock);
				sendQueue[sock_info.idx].emplace_back(PERMIT_EXIT);
				break;
			case NOTIFY_EXIT:
				running = false;
				closesocket(sock_info.sock);
				numPlayer--;
				break;
			case NOTIFY_START:
				readyPlayer++;

				break;

				// InGame���
			case INPUT_JUMP:
				charArr[sock_info.idx].InsertKey(VK_SPACE);
				break;
			case INPUT_EMOTION:
				emotionTime[sock_info.idx] = 1;
				recvn(sock_info.sock, (char*)&emotionNum[sock_info.idx], sizeof(emotionNum[0]), 0);
				break;
			case INPUT_KEY_Q:
				itemQueue.emplace_back(basicInfo.m_itemInfo[sock_info.idx][0], sock_info.idx);
				basicInfo.m_itemInfo[sock_info.idx][0] = -1;
				break;
			case INPUT_KEY_W:
				itemQueue.emplace_back(basicInfo.m_itemInfo[sock_info.idx][1], sock_info.idx);
				basicInfo.m_itemInfo[sock_info.idx][1] = -1;
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI ListenThread(LPVOID arg)
{
	// socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);

	// bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(HOSTPORT);
	int retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));

	// listen()
	retval = listen(listen_sock, SOMAXCONN);

	while (true)
	{
		// accept()
		SOCKET newSock = accept(listen_sock, NULL, NULL);
		if (newSock == ACCEPT_DENIED) break;
		int idx = GetEmptySlot();
		if (idx == -1)
		{
			closesocket(newSock);
			continue;
		}

		// �ε��� ���� ������
		send(newSock, (char*)&idx, sizeof(idx), 0);

		roomInfo.m_charInfo[idx] = CharacterName::Archer;
		numPlayer++;

		// Send����
		CreateThread(NULL, 0, SendData, (LPVOID)&RoomInfoStruct(idx, newSock), 0, NULL);

		// Recv����
		newSock = accept(listen_sock, NULL, NULL);
		CreateThread(NULL, 0, RecvData, (LPVOID)&RoomInfoStruct(idx, newSock), 0, NULL);

		// ���� �� ���� ����
		for (int i = 1; i < MAX_PLAYER; ++i)
		{
			if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
			sendQueue[i].emplace_back(UPDATE_ROOM);
		}
	}
	return 0;
}


DWORD WINAPI RecvDataGuest(LPVOID arg)
{
	RoomInfoStruct sock_info = *(RoomInfoStruct*)arg;

	int retval, op;
	bool running = true;

	while (running)
	{
		retval = recvn(sock_info.sock, (char*)&op, sizeof(op), 0);
		switch (op)
		{
			// InRoom ���
		case UPDATE_ROOM:
			retval = recvn(sock_info.sock, (char*)&roomInfo, sizeof(UpdateRoomStruct), 0);
			break;
		case NOTIFY_CHANGEMAP:
			retval = recvn(sock_info.sock, (char*)&roomInfo.m_mapInfo, sizeof(roomInfo.m_mapInfo), 0);
			break;
		case PERMIT_CHANGECHAR:
		{
			PermitChangeCharStruct chaInfo;
			retval = recvn(sock_info.sock, (char*)&chaInfo, sizeof(chaInfo), 0);
			roomInfo.m_charInfo[chaInfo.m_idx] = chaInfo.m_charInfo;
			break;
		}
		case PERMIT_EXIT:
			recvn(sock_info.sock, (char*)&running, sizeof(running), 0);
			break;
		case NOTIFY_EXIT:
			sendQueueGuest.push_back(NOTIFY_EXIT);
			closesocket(sock_info.sock);
			running = false;
			break;
		case NOTIFY_START:
			// ���ӽ���
			std::cout << "������ �����մϴ�" << std::endl;
			gameStart = true;
			break;

			// InGame ���
		case UPDATE_FRAME:
			recvn(sock_info.sock, (char*)&basicInfo, sizeof(basicInfo), 0);
			break;
		case NOTIFY_ITEM_THUNDER:
			// �Ҹ� ���
			break;
		case NOTIFY_ITEM_BED:
			// �Ҹ� ���
			break;
		case NOTIFY_ITEM_SHIELD:
			// �Ҹ� ���
			break;
		case NOTIFY_ITEM_WING:
			// �Ҹ� ���
			break;
		}
	}
	return 0;
}

DWORD WINAPI SendDataGuest(LPVOID arg)
{
	RoomInfoStruct sock_info = *(RoomInfoStruct*)arg;

	int retval, op;
	bool running = true;


	while (running)
	{
		if (!sendQueueGuest.empty())
		{
			op = sendQueueGuest.front();
			sendQueueGuest.pop_front();
			send(sock_info.sock, (char*)&op, sizeof(op), 0);
			switch (op)
			{
				// InRoom ���
			case REQUEST_CHANGECHAR:
				retval = send(sock_info.sock, (char*)&reqChar, sizeof(reqChar), 0);
				break;
			case REQUEST_EXIT:
				closesocket(sock_info.sock);
				running = false;
				break;
			case NOTIFY_EXIT:
				closesocket(sock_info.sock);
				hThreadGuest[0] = NULL;
				running = false;
				break;

				// InGame ���
			case INPUT_EMOTION:
				send(sock_info.sock, (char*)&emotionNum[sock_info.idx], sizeof(emotionNum[0]), 0);
				break;
			}
		}
	}
	return 0;
}