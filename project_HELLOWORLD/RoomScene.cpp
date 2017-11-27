#include "RoomScene.h"



DWORD WINAPI ListenThread(LPVOID arg);
DWORD WINAPI RecvData(LPVOID arg);
DWORD WINAPI SendData(LPVOID arg);

SOCKET listen_sock = NULL;

RoomScene::RoomScene()
{
}

RoomScene::RoomScene(HWND hWnd) : Scene(hWnd)
{
	readyPlayer = 0;

	CreateThread(NULL, 0, ListenThread, NULL, NULL, NULL);
	if (numPlayer == 0)
	{
		numPlayer = 1;
		roomInfo.mapInfo = MapName::Sea;
		roomInfo.charInfo[0] = CharacterName::Archer;
		for (int i = 1; i < 4; ++i)
		{
			roomInfo.charInfo[i] = CharacterName::NONE;
		}
	}
	sendQueue->clear();
	LoadCImage();
}

RoomScene::~RoomScene()
{
}


void RoomScene::Draw(HDC hdc) {
	if(roomInfo.mapInfo == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (roomInfo.mapInfo == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomScene::DrawCharacter(HDC hdc) {
	int x, y;

	for (int i = 0; i < 4; ++i)
	{
		if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
		x = GetXPosByIdx(i);	y = GetYPosByIdx(i);
		if (roomInfo.charInfo[i] == CharacterName::Archer)
			m_ImgArcher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.charInfo[i] == CharacterName::Zombie)
			m_ImgZombie.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.charInfo[i] == CharacterName::Knight)
			m_ImgKnight.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.charInfo[i] == CharacterName::Wicher)
			m_ImgWicher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
	}
}

int RoomScene::GetXPosByIdx(int idx)
{
	// Red 팀
	if (idx == 0) return 50;
	else if (idx == 1) return 200;
	// Blue 팀
	else if (idx == 2) return 600;
	else if (idx == 3) return 750;
	return -1;	// 오류
}

int RoomScene::GetYPosByIdx(int idx)
{
	// Red 팀
	if (idx == 0) return 90;
	else if (idx == 1) return 50;
	// Blue 팀
	else if (idx == 2) return 50;
	else if (idx == 3) return 90;
	return -1;	// 오류
}

bool RoomScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY > 80 && mouseY < 195 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 아처 골랐습니다." << std::endl;
			roomInfo.charInfo[0] = CharacterName::Archer;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(CHACHNG, 0);
			}
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 위쳐 골랐습니다." << std::endl;
			roomInfo.charInfo[0] = CharacterName::Wicher;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(CHACHNG, 0);
			}
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 기사 골랐습니다." << std::endl;
			roomInfo.charInfo[0] = CharacterName::Knight;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(CHACHNG, 0);
			}
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 좀비 골랐습니다." << std::endl;
			roomInfo.charInfo[0] = CharacterName::Zombie;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(CHACHNG, 0);
			}
		}


		else if (mouseY > 440 && mouseY < 485 && mouseX > 1010 && mouseX < 1060 ||
			mouseY > 440 && mouseY < 485 && mouseX > 1210 && mouseX < 1250) {
			std::cout << "나는 맵을 바꿧습니다." << std::endl;
			
			if (roomInfo.mapInfo == MapName::Sea)
				roomInfo.mapInfo = MapName::Forest;
			else if (roomInfo.mapInfo == MapName::Forest)
				roomInfo.mapInfo = MapName::Sea;

			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(MAPCHNG);
			}
		}


		else if (mouseY > 277  && mouseY < 395 && mouseX > 448 && mouseX < 565 ) {
			if (numPlayer == MAX_PLAYER)
			{
				// 게임시작
				std::cout << "게임을 시작합니다" << std::endl;

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					sendQueue[i].emplace_back(NOTIFYSTART);
				}
				readyPlayer++;
				while (readyPlayer < MAX_PLAYER);
				
				m_isDestory = true;
				m_nextScene = SceneName::InGame;
			}
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// 나가기
			std::cout << "나가요!! 로비로!!!! " << std::endl;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(NOTIFYEXIT);
			}
			numPlayer--;
			while (numPlayer > 0);
			closesocket(listen_sock);
			m_isDestory = true;
			m_nextScene = SceneName::Lobby;
		}

		return true;
	}
}

void RoomScene::Timer(const double count) {

}


void RoomScene::LoadCImage() {
	m_backImg[0].Load("Resource/Image/Room/GameRoom_host_1.png");	//Sea
	m_backImg[1].Load("Resource/Image/Room/GameRoom_host_2.png");	//Forest

	m_ImgArcher.Load("Resource/Image/Room/Room_Archer.png");
	m_ImgZombie.Load("Resource/Image/Room/Room_Zombie.png");
	m_ImgKnight.Load("Resource/Image/Room/Room_Knight.png");
	m_ImgWicher.Load("Resource/Image/Room/Room_Wicher.png");

}


int GetEmptySlot()
{
	for (int i = 0; i < MAX_PLAYER; ++i)
		if (roomInfo.charInfo[i] == CharacterName::NONE) return i;
	return -1;
}

DWORD WINAPI SendData(LPVOID arg)
{
	RoomConnect sock_info = *(RoomConnect*)arg;

	int retval;
	bool running = true;

	QueueData sendOp;
	
	sendQueue[sock_info.idx].clear();

	// 인덱스 정보 보내기
	send(sock_info.sock, (char*)&sock_info.idx, sizeof(sock_info.idx), 0);

	while (running)
	{
		if (!sendQueue[sock_info.idx].empty())
		{
			sendOp = sendQueue[sock_info.idx].front();
			sendQueue[sock_info.idx].pop_front();
			send(sock_info.sock, (char*)&sendOp.op, sizeof(sendOp.op), 0);
			switch (sendOp.op)
			{
				// InRoom통신
			case ROOMCHNG:
				send(sock_info.sock, (char*)&roomInfo, sizeof(roomInfo), 0);
				break;
			case MAPCHNG:
				send(sock_info.sock, (char*)&roomInfo.mapInfo, sizeof(roomInfo.mapInfo), 0);
				break;
			case CHACHNG:
			{
				ChaChng chaInfo(sendOp.fromIdx, roomInfo.charInfo[sendOp.fromIdx]);
				send(sock_info.sock, (char*)&chaInfo, sizeof(chaInfo), 0);
				break;
			}
			case REQEXIT:
				running = false;
				roomInfo.charInfo[sock_info.idx] = CharacterName::NONE;

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
					sendQueue[i].emplace_back(ROOMCHNG);
				}

				send(sock_info.sock, (char*)&running, sizeof(running), 0);
				closesocket(sock_info.sock);
				numPlayer--;
				break;
			case NOTIFYEXIT:
				running = false;
				closesocket(sock_info.sock);
				break;

				// InGame 통신
			case BASICINFO:
				basicInfo.platInfo = platFirst[sock_info.idx];
				send(sock_info.sock, (char*)&basicInfo, sizeof(basicInfo), 0);
				break;
			}
		}
	}

	return 0;
}

DWORD WINAPI RecvData(LPVOID arg)
{
	RoomConnect sock_info = *(RoomConnect*)arg;

	int retval, op;
	bool running = true;

	while (running)
	{
		retval = recvn(sock_info.sock, (char*)&op, sizeof(op), 0);
		if (retval > 0)
		{
			switch (op)
			{
			case -1:	// 타임아웃
				break;
				// InRoom 통신
			case CHACHNG:
				retval = recvn(sock_info.sock, (char*)&roomInfo.charInfo[sock_info.idx], sizeof(roomInfo.charInfo[sock_info.idx]), 0);

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
					sendQueue[i].emplace_back(CHACHNG, sock_info.idx);
				}
				break;
			case REQEXIT:
				running = false;
				closesocket(sock_info.sock);
				sendQueue[sock_info.idx].emplace_back(REQEXIT);
				break;
			case NOTIFYEXIT:
				running = false;
				closesocket(sock_info.sock);
				numPlayer--;
				break;
			case NOTIFYSTART:
				readyPlayer++;
				break;

				// InGame통신
			case INPUT_JUMP:
				charArr[sock_info.idx].InsertKey(VK_SPACE);
				break;
			case INPUT_EMOTION:
				emotionTime[sock_info.idx] = 1;
				recvn(sock_info.sock, (char*)&emotionNum[sock_info.idx], sizeof(emotionNum[0]), 0);
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

	// 데이터 통신에 사용할 변수
	SOCKADDR_IN clientaddr;
	RoomConnect threadInfo;
	int addrlen;

	while (true)
	{
		// accept()
		addrlen = sizeof(clientaddr);
		SOCKET sendSock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (sendSock == ACCEPT_DENIED) break;
		int idx = GetEmptySlot();
		if (idx == -1)
		{
			closesocket(sendSock);
			continue;
		}
		roomInfo.charInfo[idx] = CharacterName::Archer;
		numPlayer++;
		// Send생성
		threadInfo = RoomConnect(idx, sendSock);
		CreateThread(NULL, 0, SendData, (LPVOID)&threadInfo, 0, NULL);
		SOCKET recvSock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		// Recv생성
		threadInfo = RoomConnect(idx, recvSock);
		CreateThread(NULL, 0, RecvData, (LPVOID)&threadInfo, 0, NULL);

		// 변한 방 정보 전송
		for (int i = 1; i < MAX_PLAYER; ++i)
		{
			if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
			sendQueue[i].emplace_back(ROOMCHNG);
		}
	}
	return 0;
}