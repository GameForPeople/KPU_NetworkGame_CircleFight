#include "RoomSceneGuest.h"


DWORD WINAPI RecvDataGuest(LPVOID arg);
DWORD WINAPI SendDataGuest(LPVOID arg);

HANDLE hThreadGuest[2];
CharacterName hopCha;

RoomSceneGuest::RoomSceneGuest()
{
}

RoomSceneGuest::RoomSceneGuest(HWND hWnd) : Scene(hWnd)
{
	sendQueueGuest.clear();

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", (PVOID *)(&serveraddr.sin_addr.s_addr));
	serveraddr.sin_port = htons(HOSTPORT);

	int retval;
	// recv소켓 연결 시도
	SOCKET newSock = socket(AF_INET, SOCK_STREAM, 0);
	if (newSock == INVALID_SOCKET) err_quit("socket()");
	connect(newSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	retval = recvn(newSock, (char*)&m_idx, sizeof(m_idx), 0);
	if (retval > 0)
	{
		hThreadGuest[0] = CreateThread(NULL, 0, RecvDataGuest, (LPVOID)&RoomConnect(m_idx, newSock), 0, NULL);
		//send소켓 연결 시도
		newSock = socket(AF_INET, SOCK_STREAM, 0);
		if (newSock == INVALID_SOCKET) err_quit("socket()");
		connect(newSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		hThreadGuest[1] = CreateThread(NULL, 0, SendDataGuest, (LPVOID)&RoomConnect(m_idx, newSock), 0, NULL);
	}

	if (retval <= 0)
	{
		// 방 입장 불가능
		cout << "방 입장 불가능" << endl;
		m_isDestory = true;
		m_nextScene = SceneName::Lobby;
	}

	LoadCImage();
}

RoomSceneGuest::~RoomSceneGuest()
{
}


void RoomSceneGuest::Draw(HDC hdc) {
	if (roomInfoGuest.mapInfo == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (roomInfoGuest.mapInfo == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomSceneGuest::DrawCharacter(HDC hdc) {
	int x, y;

	for (int i = 0; i < 4; ++i)
	{
		if (roomInfoGuest.charInfo[i] == CharacterName::NONE) continue;
		x = GetXPosByIdx(i);	y = GetYPosByIdx(i);
		if (roomInfoGuest.charInfo[i] == CharacterName::Archer)
			m_ImgArcher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfoGuest.charInfo[i] == CharacterName::Zombie)
			m_ImgZombie.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfoGuest.charInfo[i] == CharacterName::Knight)
			m_ImgKnight.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfoGuest.charInfo[i] == CharacterName::Wicher)
			m_ImgWicher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
	}
}

int RoomSceneGuest::GetXPosByIdx(int idx)
{
	// Red 팀
	if (idx == 0) return 50;
	else if (idx == 1) return 200;
	// Blue 팀
	else if (idx == 2) return 600;
	else if (idx == 3) return 750;
	return -1;	// 오류
}

int RoomSceneGuest::GetYPosByIdx(int idx)
{
	// Red 팀
	if (idx == 0) return 90;
	else if (idx == 1) return 50;
	// Blue 팀
	else if (idx == 2) return 50;
	else if (idx == 3) return 90;
	return -1;	// 오류
}


bool RoomSceneGuest::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY > 80 && mouseY < 195 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 아처 골랐습니다." << std::endl;
			hopCha = CharacterName::Archer;
			sendQueueGuest.push_back(CHACHNG);
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 위쳐 골랐습니다." << std::endl;
			hopCha = CharacterName::Wicher;
			sendQueueGuest.push_back(CHACHNG);
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 기사 골랐습니다." << std::endl;
			hopCha = CharacterName::Knight;
			sendQueueGuest.push_back(CHACHNG);
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 좀비 골랐습니다." << std::endl;
			hopCha = CharacterName::Zombie;
			sendQueueGuest.push_back(CHACHNG);
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// 나가기
			std::cout << "나가요!! 로비로!!!! " << std::endl;
			sendQueueGuest.push_back(REQEXIT);
			WaitForMultipleObjects(2, hThreadGuest, TRUE, INFINITE);
			m_isDestory = true;
			m_nextScene = SceneName::Lobby;
		}

		return true;
	}
}

void RoomSceneGuest::Timer(const double count) {
	if (!hThreadGuest[0])
	{
		m_isDestory = true;
		m_nextScene = SceneName::Lobby;
	}
	else if (gameStart)
	{
		m_isDestory = true;
		m_nextScene = SceneName::InGameGuest;
	}
}


void RoomSceneGuest::LoadCImage() {
	m_backImg[0].Load("Resource/Image/Room/GameRoom_guest_1.png");	//Sea
	m_backImg[1].Load("Resource/Image/Room/GameRoom_guest_2.png");	//Forest

	m_ImgArcher.Load("Resource/Image/Room/Room_Archer.png");
	m_ImgZombie.Load("Resource/Image/Room/Room_Zombie.png");
	m_ImgKnight.Load("Resource/Image/Room/Room_Knight.png");
	m_ImgWicher.Load("Resource/Image/Room/Room_Wicher.png");

}


DWORD WINAPI RecvDataGuest(LPVOID arg)
{
	RoomConnect sock_info = *(RoomConnect*)arg;

	int retval, op;
	bool running = true;
	sendQueueGuest.clear();

	while (running)
	{
		retval = recvn(sock_info.sock, (char*)&op, sizeof(op), 0);
		switch (op)
		{
			// InRoom 통신
		case ROOMCHNG:
			retval = recvn(sock_info.sock, (char*)&roomInfoGuest, sizeof(RoomInfo), 0);
			break;
		case MAPCHNG:
			retval = recvn(sock_info.sock, (char*)&roomInfoGuest.mapInfo, sizeof(roomInfoGuest.mapInfo), 0);
			break;
		case CHACHNG:
		{
			ChaChng chaInfo;
			retval = recvn(sock_info.sock, (char*)&chaInfo, sizeof(chaInfo), 0);
			roomInfoGuest.charInfo[chaInfo.idx] = chaInfo.charInfo;
			break;
		}
		case REQEXIT:
			recvn(sock_info.sock, (char*)&running, sizeof(running), 0);
			break;
		case NOTIFYEXIT:
			sendQueueGuest.push_back(NOTIFYEXIT);
			closesocket(sock_info.sock);
			running = false;
			break;
		case NOTIFYSTART:
			// 게임시작
			std::cout << "게임을 시작합니다" << std::endl;
			gameStart = true;
			break;

			// InGame 통신
		case BASICINFO:
			recvn(sock_info.sock, (char*)&basicInfo, sizeof(basicInfo), 0);
			break;
		}
	}
	return 0;
}

DWORD WINAPI SendDataGuest(LPVOID arg)
{
	RoomConnect sock_info = *(RoomConnect*)arg;

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
				// InRoom 통신
			case CHACHNG:
				retval = send(sock_info.sock, (char*)&hopCha, sizeof(hopCha), 0);
				break;
			case REQEXIT:
			case NOTIFYEXIT:
				closesocket(sock_info.sock);
				hThreadGuest[0] = NULL;
				running = false;
				break;

				// InGame 통신
			case INPUT_EMOTION:
				send(sock_info.sock, (char*)&emotionNum[sock_info.idx], sizeof(emotionNum[0]), 0);
				break;
			}
		}
	}
	return 0;
}