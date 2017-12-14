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
	InitializeCriticalSection(&CHANGE_FLAG_SECTION);
}

Network::~Network()
{
	//DeleteCriticalSection(&SEND_SECTION);
	DeleteCriticalSection(&LOBBY_UPDATE_SECTION);
	DeleteCriticalSection(&CHANGE_FLAG_SECTION);
}


void Network::NetworkThreadFunction() {
	std::cout << "Hello League of Runners! I'm your New Thread!" << std::endl;
	while (7) {
		if (m_sceneName == SceneName::Login) {
			if (m_sendType) {
				#ifdef DEBUG_MODE
				std::cout << " DEMAND_LOGIN�� �����մϴ�." << std::endl;
				#endif
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				if (m_sendType == DEMAND_LOGIN) {

				#ifdef DEBUG_MODE
				std::cout << "�α��� �Ǵ�  ȸ������ ���� ������!" << std::endl;
				std::cout << "������ Ÿ����  " << m_demandLogin->type << std::endl;
				std::cout << "������ ID��  " << m_demandLogin->ID << std::endl;
				std::cout << "������ PW��  " << m_demandLogin->PW<< std::endl;
				std::cout << "������ SIZE�� *m_demandLogin " << sizeof(*m_demandLogin) << std::endl;
				std::cout << "������ SIZE�� m_demandLogin " << sizeof(m_demandLogin) << std::endl;
				std::cout << "������ SIZE�� &m_demandLogin " << sizeof(&m_demandLogin) << std::endl;

				std::cout << "�α��� ���� �Ǵ� ���������ϴ�.!" << std::endl;
				#endif

				//retVal = send(m_sock, (char*)&m_demandLogin, sizeof(m_demandLogin), 0);
				retVal = send(m_sock, (char*)m_demandLogin, sizeof(*m_demandLogin), 0);

				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				#ifdef DEBUG_MODE
				std::cout << "�α��� ���� �Ǵ� ���� ���¾��!" << std::endl;
				#endif

				int m_recvTypeBuffer{};
				retVal = recv(m_sock, (char*)&m_recvTypeBuffer, sizeof(m_recvType), 0);
				if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

				m_sendType = 0;

				if (m_recvTypeBuffer == PERMIT_LOGIN) {
					
					if(m_permitLogin == NULL) m_permitLogin = new PermitLoginStruct;

					retVal = recv(m_sock, (char*)m_permitLogin, sizeof(*m_permitLogin), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					#ifdef DEBUG_MODE
					std::cout << "���� �¸� Ƚ���� " << m_permitLogin->winCount << std::endl;
					std::cout << "���� �й� Ƚ���� " << m_permitLogin->loseCount << std::endl;
					std::cout << "���� �������� ũ��� " << sizeof(*m_permitLogin) << std::endl;
					#endif

					}
				
				m_recvType = m_recvTypeBuffer;
				}
			}
		}
		else if (m_sceneName == SceneName::Lobby) {
			//_sleep(100);
			CustomSleep(100);

			#ifdef DEBUG_MODE
						std::cout << "server�� ���޵Ǵ� ���� : "<< m_sendType << " ";
			#endif

			if (m_sendType > 0) {
				#ifdef DEBUG_MODE
				std::cout << "�κ��Դϴ�. �䱸�ҰԿ� ������!" << std::endl;
				#endif

				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;
				

				if (m_sendType == DEMAND_CHAT) {

					retVal = send(m_sock, (char*)m_demandChat, sizeof(*m_demandChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					// ä�� ���� ���� -> �� ���� �ʿ� �� , ���ڵ� ��Ȱ!!!!!!!!!!!
					//if(m_permitChat == NULL)
					//	m_permitChat = new PermitChatStruct;
					//
					//retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					//if (!ErrorFunction(retVal, 0)) goto END_CONNECT;
					
					//std::cout << m_permitChat->chat[0] << std::endl;
					//std::cout << m_permitChat->chat[1] << std::endl;
					//std::cout << m_permitChat->chat[2] << std::endl;
					//std::cout << m_permitChat->chat[3] << std::endl;
					//std::cout << m_permitChat->chat[4] << std::endl;

					m_sendType = 0;
					m_recvType = 1;
				}
				else if (m_sendType == DEMAND_CREATEROOM) {
					int m_recvTypeBuffer{};

					retVal = recv(m_sock, (char*)&m_recvTypeBuffer, sizeof(m_recvTypeBuffer), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					if (m_recvTypeBuffer == PERMIT_CREATEROOM) {
						if (m_permitCreateRoom == NULL)
							m_permitCreateRoom = new PermitCreateRoomStruct;

						retVal = recv(m_sock, (char*)m_permitCreateRoom, sizeof(*m_permitCreateRoom), 0);
					
						if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

						m_sendType = 0;
						m_recvType = m_recvTypeBuffer;
					}
					else if (m_recvTypeBuffer == FAIL_CREATEROOM) {
						m_sendType = 0;
						m_recvType = m_recvTypeBuffer;
					}
				}
				else if (m_sendType == DEMAND_JOINROOM) {
					retVal = send(m_sock, (char*)m_demandJoinRoom, sizeof(m_demandJoinRoom), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					int m_recvTypeBuffer{};
					retVal = recv(m_sock, (char*)&m_recvTypeBuffer, sizeof(m_recvTypeBuffer), 0);
					if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

					if (m_recvTypeBuffer == PERMIT_JOINROOM) {
						if(m_permitJoinRoom == NULL) m_permitJoinRoom = new PermitJoinRoomStruct;

						retVal = recv(m_sock, (char*)m_permitJoinRoom, sizeof(m_permitJoinRoom), 0);
						if (!ErrorFunction(retVal, 0)) goto END_CONNECT;

						m_sendType = 0;
						m_recvType = m_recvTypeBuffer;
					}
					else if (m_recvTypeBuffer == FAIL_JOINROOM) {
						m_sendType = 0;
						m_recvType = FAIL_JOINROOM;
					}
				}
			}
			else if(m_sendType == 0){

				while (7) {
					EnterCriticalSection(&CHANGE_FLAG_SECTION);
					if (m_flag == 0) {
						m_flag = 1;
						LeaveCriticalSection(&CHANGE_FLAG_SECTION);
						break;
					}
					LeaveCriticalSection(&CHANGE_FLAG_SECTION);
					CustomSleep(15);
				}

				//EnterCriticalSection(&LOBBY_UPDATE_SECTION);
					m_sendType = UPDATE_LOBBY;

					retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					if(m_permitChat == NULL)
						m_permitChat = new PermitChatStruct;

					retVal = recv(m_sock, (char*)m_permitChat, sizeof(*m_permitChat), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					#ifdef DEBUG_MODE
					//std::cout << "m_permitChat->chat[0] : " << m_permitChat->chat[0] << std::endl;
					//std::cout << "m_permitChat->chat[1] : " << m_permitChat->chat[1] << std::endl;
					//std::cout << "m_permitChat->chat[2] : " << m_permitChat->chat[2] << std::endl;
					//std::cout << "m_permitChat->chat[3] : " << m_permitChat->chat[3] << std::endl;
					//std::cout << "m_permitChat->chat[4] : " << m_permitChat->chat[4] << std::endl;
					#endif

					if (m_updateLobbyInfo == NULL)
						m_updateLobbyInfo = new UpdateLobbyInfoStruct;

					retVal = recv(m_sock, (char*)m_updateLobbyInfo, sizeof(*m_updateLobbyInfo), 0);
					if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

					m_sendType = 0;
					m_recvType = 0;
					m_flag = 0;

				//LeaveCriticalSection(&LOBBY_UPDATE_SECTION);
			}
		}
		else if (m_sceneName == SceneName::RoomGuest) {
			if (m_sendType == DEMAND_EXITROOM) {
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				m_sceneName = SceneName::Lobby;
				m_sendType = 0;
			}
			if (m_gameResult) {
				int sendTypeBuffer = DEMAND_SENDRESULT;
				retVal = send(m_sock, (char*)&sendTypeBuffer, sizeof(sendTypeBuffer), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				retVal = send(m_sock, (char*)&m_gameResult, sizeof(m_gameResult), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				m_gameResult = 0;
			}
		}
		else if (m_sceneName == SceneName::Room) {
			if (m_sendType == DEMAND_EXITROOM) {
				retVal = send(m_sock, (char*)&m_sendType, sizeof(m_sendType), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				m_sceneName = SceneName::Lobby;
				m_sendType = 0;
			}
			if (m_gameResult) {
				int sendTypeBuffer = DEMAND_SENDRESULT;
				retVal = send(m_sock, (char*)&sendTypeBuffer, sizeof(sendTypeBuffer), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				retVal = send(m_sock, (char*)&m_gameResult, sizeof(m_gameResult), 0);
				if (!ErrorFunction(retVal, 1)) goto END_CONNECT;

				m_gameResult = 0;
			}
		}
		else {
			CustomSleep(1000);
		}
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

#ifdef DEBUG_MODE
	std::cout << "   ToDebug :  Return Value�� ���������մϴ�." << std::endl;
#endif

	retVal = recv(m_sock, (char*)&type, sizeof(type), 0);
	if (!ErrorFunction(retVal, 0))
		return 0;

#ifdef DEBUG_MODE
	std::cout << "   ToDebug :  Return Value�� �޾ҽ��ϴ�. �� ���� : " << type << std::endl;
#endif

	return type;
}

void Network::InitSound() {
	System_Create(&m_system); //	���� �ý��� �⺻ ����
	m_system->init(1, FMOD_INIT_NORMAL, NULL); //	�������� ����

	m_system->createSound(
		"Resource//Sound//bgm1.mp3",	//	�������ϰ��
		FMOD_HARDWARE | FMOD_LOOP_NORMAL,	//	�������Ͽɼ�
		NULL,			//	Ȯ�嵥����(��������)
		&m_sound[0]);	//	������ ���� ������

	m_system->createSound(
		"Resource//Sound//bgm2.mp3",
		FMOD_HARDWARE | FMOD_LOOP_NORMAL,
		NULL,
		&m_sound[1]);

	m_channel[0]->isPlaying(&IsBGPlaying);

	//BGM���
	//pSystem->playSound(FMOD_CHANNEL_REUSE, pSound[1], false, &pChannel[0]);

	//ȿ���� ���
	//PlaySound("Resource\\sound\\���~.wav", NULL, SND_ASYNC);
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

void Network::CustomSleep(int milliSecond) {
	clock_t start_clk = clock();  // ���� �ð��� ���Ѵ�.

	milliSecond--;

	while (1) {
		// ���������� clock �Լ��� ȣ���Ͽ� �귯�� �ð��� ����Ҳ���������������!!!
		if ((clock() - start_clk) > milliSecond) break;
	}
}

deque<QueueData> sendQueue[MAX_PLAYER];
deque<int> sendQueueGuest;
int m_idx;
UpdateFrameStruct basicInfo;
Pawn* charArr;
int	emotionNum[MAX_PLAYER];
int  emotionTime[MAX_PLAYER];

atomic<int> numPlayer = 0;
UpdateRoomStruct roomInfo;
atomic<int> readyPlayer;
SOCKET listen_sock = NULL;

int setItemKind[MAX_PLAYER];
int resetItemKind[MAX_PLAYER];
//atomic<bool> listenThreadExit = false;
//atomic<bool> RecvThreadExit = false;
//atomic<bool> SendThreadExit = false;

bool gameStart;
HANDLE hThreadGuest[2];
CharacterName reqChar;
deque<UseItemInfo> itemQueue;

Network* threadNetwork;

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
			if (sendOp.op != SOCKET_ERROR)
			{
				retval = send(sock_info.sock, (char*)&sendOp.op, sizeof(sendOp.op), 0);
				if (retval == SOCKET_ERROR) sendOp.op = SOCKET_ERROR;
			}
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
			case CHANGE_EMOTION:
				send(sock_info.sock, (char*)&ChangeEmotionStruct(emotionNum[sendOp.fromIdx], sendOp.fromIdx), sizeof(ChangeEmotionStruct), 0);
				break;
			case SET_UI_THUNDER:
			case SET_UI_BED:
			case SET_UI_SHEILD:
			case SET_UI_WING:
			case RESET_UI_THUNDER:
			case RESET_UI_BED:
			case RESET_UI_SHEILD:
			case RESET_UI_WING:
				send(sock_info.sock, (char*)&sendOp.fromIdx, sizeof(sendOp.fromIdx), 0);
				break;

				// ����ó��
			case SOCKET_ERROR:
				running = false;
				roomInfo.m_charInfo[sock_info.idx] = CharacterName::NONE;
				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
					sendQueue[i].emplace_back(UPDATE_ROOM);
				}
				numPlayer--;
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
		if (retval == SOCKET_ERROR)
			op = SOCKET_ERROR;
		switch (op)
		{
			// InRoom ���
		case REQUEST_CHANGECHAR:
			retval = recvn(sock_info.sock, (char*)&roomInfo.m_charInfo[sock_info.idx], sizeof(roomInfo.m_charInfo[sock_info.idx]), 0);
			if (retval == SOCKET_ERROR)
				op = SOCKET_ERROR;
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
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (i == sock_info.idx)continue;
				sendQueue[i].emplace_back(CHANGE_EMOTION, sock_info.idx);
			}
			break;
		case INPUT_KEY_Q:
			itemQueue.emplace_back(basicInfo.m_itemInfo[sock_info.idx][0], sock_info.idx);
			basicInfo.m_itemInfo[sock_info.idx][0] = -1;
			break;
		case INPUT_KEY_W:
			itemQueue.emplace_back(basicInfo.m_itemInfo[sock_info.idx][1], sock_info.idx);
			basicInfo.m_itemInfo[sock_info.idx][1] = -1;
			break;

			// ���� ó��
		case SOCKET_ERROR:
			running = false;
			sendQueue[sock_info.idx].emplace_back(SOCKET_ERROR);
			break;
		}

		
	}

	return 0;
}

DWORD WINAPI ListenThread(LPVOID arg)
{
	RoomInfoStruct recvStruct;
	RoomInfoStruct sendStruct;
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
		if (newSock > ACCEPT_DENIED) break;
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
		sendStruct.idx = idx;		sendStruct.sock = newSock;
		CreateThread(NULL, 0, SendData, (LPVOID)&sendStruct, 0, NULL);

		// Recv����
		newSock = accept(listen_sock, NULL, NULL);
		recvStruct.idx = idx;		recvStruct.sock = newSock;
		CreateThread(NULL, 0, RecvData, (LPVOID)&recvStruct, 0, NULL);

		// ���� �� ���� ����
		for (int i = 1; i < MAX_PLAYER; ++i)
		{
			if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
			sendQueue[i].emplace_back(UPDATE_ROOM, 0);
		}

	}

	return 0;
}


DWORD WINAPI RecvDataGuest(LPVOID arg)
{
	RoomInfoStruct sock_info = *(RoomInfoStruct*)arg;

	int retval, op;
	int itemUser;
	int b, e;
	bool running = true;

	while (running)
	{
		retval = recvn(sock_info.sock, (char*)&op, sizeof(op), 0);
		if (retval == SOCKET_ERROR)
			op = SOCKET_ERROR;
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
			std::cout << "������ ���۵˴ϴ�" << std::endl;
			gameStart = true;
			break;

			// InGame ���
		case UPDATE_FRAME:
			recvn(sock_info.sock, (char*)&basicInfo, sizeof(basicInfo), 0);
			break;
		case NOTIFY_ITEM_THUNDER:
			// �Ҹ� ���	
			PlaySound("Resource\\Sound\\ets.wav", NULL, SND_ASYNC);
			break;
		case NOTIFY_ITEM_BED:
			// �Ҹ� ���
			PlaySound("Resource\\Sound\\sleeping.wav", NULL, SND_ASYNC);
			break;
		case NOTIFY_ITEM_SHIELD:
			// �Ҹ� ���
			PlaySound("Resource\\Sound\\shield.wav", NULL, SND_ASYNC);
			break;
		case NOTIFY_ITEM_WING:
			// �Ҹ� ���
			PlaySound("Resource\\Sound\\angle.wav", NULL, SND_ASYNC);
			break;
		case CHANGE_EMOTION:
		{
			ChangeEmotionStruct ces;
			recvn(sock_info.sock, (char*)&ces, sizeof(ces), 0);
			emotionNum[ces.userIdx] = ces.emotionNum;
		}
		break;
		case NOTIFY_WIN:
			threadNetwork->m_gameResult = 1;
			threadNetwork->m_gameResultBuffer = 1;
			ResumeThread(threadNetwork->m_networkThread);
			break;
		case NOTIFY_LOSE:
			threadNetwork->m_gameResult = 2;
			threadNetwork->m_gameResultBuffer = 2;
			ResumeThread(threadNetwork->m_networkThread);
			break;
		case SET_UI_THUNDER:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			setItemKind[itemUser] = LIGHTNING;
			break;
		case SET_UI_BED:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			setItemKind[itemUser] = BED;
			break;
		case SET_UI_SHEILD:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			if (itemUser < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }
			for (; b < e; ++b) { setItemKind[b] = SHEILD; }
			break;
		case SET_UI_WING:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			if (itemUser < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }
			for (; b < e; ++b) { setItemKind[b] = WING; }
			break;
		case RESET_UI_THUNDER:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			resetItemKind[itemUser] = LIGHTNING;
			break;
		case RESET_UI_BED:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			resetItemKind[itemUser] = BED;
			break;
		case RESET_UI_SHEILD:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			if (itemUser < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }
			for (; b < e; ++b) { resetItemKind[b] = SHEILD; }
			break;
		case RESET_UI_WING:
			recvn(sock_info.sock, (char*)&itemUser, sizeof(itemUser), 0);
			if (itemUser < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }
			for (; b < e; ++b) { resetItemKind[b] = WING; }
			break;

		// ����ó��
		case SOCKET_ERROR:
			running = false;
			closesocket(sock_info.sock);
			sendQueueGuest.emplace_back(SOCKET_ERROR);
			hThreadGuest[0] = NULL;
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
			if (op != SOCKET_ERROR)
			{
				retval = send(sock_info.sock, (char*)&op, sizeof(op), 0);
				if (retval == SOCKET_ERROR) op = SOCKET_ERROR;
			}
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

				// ���� ó��
			case SOCKET_ERROR:
				running = false;
				closesocket(sock_info.sock);
				break;
			}
		}
	}
	return 0;
}