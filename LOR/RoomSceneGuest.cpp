#include "RoomSceneGuest.h"

RoomSceneGuest::RoomSceneGuest()
{
}

RoomSceneGuest::RoomSceneGuest(HWND hWnd, Network* network) : Scene(hWnd)
{
	m_network = network;
	m_network->ChageSceneName(SceneName::RoomGuest);

	if (m_network->m_nowBgmNumber == 1) {
		m_network->m_nowBgmNumber = 0;
		m_network->m_system->playSound(FMOD_CHANNEL_REUSE, m_network->m_sound[0], false, &(m_network->m_channel[0]));
	}

	sendQueueGuest.clear();

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	
	serveraddr.sin_family = AF_INET;
	//inet_pton(AF_INET, "127.0.0.1", (PVOID *)(&serveraddr.sin_addr.s_addr));
	serveraddr.sin_port = htons(HOSTPORT);
	
	serveraddr.sin_addr = m_network->m_permitJoinRoom->hostAddr;

	#ifdef DEBUG_MODE
	std::cout << "ip : " << inet_ntoa(serveraddr.sin_addr) << std::endl;
	#endif

	int retval;
	// recv소켓 연결 시도
	SOCKET newSock = socket(AF_INET, SOCK_STREAM, 0);
	connect(newSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	retval = recvn(newSock, (char*)&m_idx, sizeof(m_idx), 0);

	#ifdef DEBUG_MODE
	std::cout << "ip : " << inet_ntoa(serveraddr.sin_addr) << std::endl;
	#endif

	if (retval > 0)
	{
		hThreadGuest[0] = CreateThread(NULL, 0, RecvDataGuest, (LPVOID)&RoomInfoStruct(m_idx, newSock), 0, NULL);
		//send소켓 연결 시도
		newSock = socket(AF_INET, SOCK_STREAM, 0);
		connect(newSock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
		hThreadGuest[1] = CreateThread(NULL, 0, SendDataGuest, (LPVOID)&RoomInfoStruct(m_idx, newSock), 0, NULL);
	}

	if (retval <= 0)
	{
		// 방 입장 불가능
		#ifdef DEBUG_MODE
		cout << "방 입장 불가능" << endl;
		#endif

		m_nextScene = SceneName::Lobby;
		m_isDestory = true;
	}

	LoadCImage();
}

RoomSceneGuest::~RoomSceneGuest()
{
}


void RoomSceneGuest::Draw(HDC hdc) {
	if (roomInfo.m_mapInfo == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (roomInfo.m_mapInfo == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomSceneGuest::DrawCharacter(HDC hdc) {
	int x, y;

	for (int i = 0; i < 4; ++i)
	{
		if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
		x = GetXPosByIdx(i);	y = GetYPosByIdx(i);
		if (roomInfo.m_charInfo[i] == CharacterName::Archer)
			m_ImgArcher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.m_charInfo[i] == CharacterName::Zombie)
			m_ImgZombie.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.m_charInfo[i] == CharacterName::Knight)
			m_ImgKnight.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (roomInfo.m_charInfo[i] == CharacterName::Wicher)
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
			#ifdef DEBUG_MODE
			std::cout << "나는 아처 골랐습니다." << std::endl;
			#endif
			reqChar = CharacterName::Archer;
			sendQueueGuest.push_back(REQUEST_CHANGECHAR);
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			#ifdef DEBUG_MODE
			std::cout << "나는 위쳐 골랐습니다." << std::endl;
			#endif
			reqChar = CharacterName::Wicher;
			sendQueueGuest.push_back(REQUEST_CHANGECHAR);
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			#ifdef DEBUG_MODE
			std::cout << "나는 기사 골랐습니다." << std::endl;
			#endif
			reqChar = CharacterName::Knight;
			sendQueueGuest.push_back(REQUEST_CHANGECHAR);
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			#ifdef DEBUG_MODE
			std::cout << "나는 좀비 골랐습니다." << std::endl;
			#endif
			reqChar = CharacterName::Zombie;
			sendQueueGuest.push_back(REQUEST_CHANGECHAR);
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// 나가기
			#ifdef DEBUG_MODE
			std::cout << "나가요!! 로비로!!!! " << std::endl;
			#endif

			sendQueueGuest.push_back(REQUEST_EXIT);
			m_network->SetSendType(DEMAND_EXITROOM);
			WaitForMultipleObjects(2, hThreadGuest, TRUE, INFINITE);
			//m_network->ChageSceneName(SceneName::Lobby);
			m_nextScene = SceneName::Lobby;
			m_isDestory = true;
		}

		return true;
	}
}

void RoomSceneGuest::Timer(const double count) {
	if (!hThreadGuest[0])
	{
		std::cout << "방에 들어가는 것이 2번째 이유로 실패했습니다. " << std::endl;

		m_network->SetSendType(DEMAND_EXITROOM);
		m_nextScene = SceneName::Lobby;
		m_isDestory = true;
	}
	else if (gameStart)
	{
		gameStart = false;
		m_nextScene = SceneName::InGameGuest;
		m_isDestory = true;
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