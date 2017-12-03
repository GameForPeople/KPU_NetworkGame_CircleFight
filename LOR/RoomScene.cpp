#include "RoomScene.h"

RoomScene::RoomScene()
{
}

RoomScene::RoomScene(HWND hWnd, Network* network) : Scene(hWnd)
{
	m_network = network;
	readyPlayer = 0;

	CreateThread(NULL, 0, ListenThread, NULL, NULL, NULL);
	if (numPlayer == 0)
	{
		for (int i = 0; i < 4; ++i)
		{
			sendQueue[i].clear();
			roomInfo.m_charInfo[i] = CharacterName::NONE;
		}

		numPlayer = 1;
		roomInfo.m_mapInfo = MapName::Sea;
		roomInfo.m_charInfo[0] = CharacterName::Archer;
	}

	LoadCImage();
}

RoomScene::~RoomScene()
{
}


void RoomScene::Draw(HDC hdc) {
	if (roomInfo.m_mapInfo == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (roomInfo.m_mapInfo == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomScene::DrawCharacter(HDC hdc) {
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

int RoomScene::GetXPosByIdx(int idx)
{
	// Red ��
	if (idx == 0) return 50;
	else if (idx == 1) return 200;
	// Blue ��
	else if (idx == 2) return 600;
	else if (idx == 3) return 750;
	return -1;	// ����
}

int RoomScene::GetYPosByIdx(int idx)
{
	// Red ��
	if (idx == 0) return 90;
	else if (idx == 1) return 50;
	// Blue ��
	else if (idx == 2) return 50;
	else if (idx == 3) return 90;
	return -1;	// ����
}

bool RoomScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY > 80 && mouseY < 195 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "���� ��ó ������ϴ�." << std::endl;
			roomInfo.m_charInfo[0] = CharacterName::Archer;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(PERMIT_CHANGECHAR, 0);
			}
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "���� ���� ������ϴ�." << std::endl;
			roomInfo.m_charInfo[0] = CharacterName::Wicher;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(PERMIT_CHANGECHAR, 0);
			}
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "���� ��� ������ϴ�." << std::endl;
			roomInfo.m_charInfo[0] = CharacterName::Knight;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(PERMIT_CHANGECHAR, 0);
			}
		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "���� ���� ������ϴ�." << std::endl;
			roomInfo.m_charInfo[0] = CharacterName::Zombie;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(PERMIT_CHANGECHAR, 0);
			}
		}


		else if (mouseY > 440 && mouseY < 485 && mouseX > 1010 && mouseX < 1060 ||
			mouseY > 440 && mouseY < 485 && mouseX > 1210 && mouseX < 1250) {
			std::cout << "���� ���� �مf���ϴ�." << std::endl;

			if (roomInfo.m_mapInfo == MapName::Sea)
				roomInfo.m_mapInfo = MapName::Forest;
			else if (roomInfo.m_mapInfo == MapName::Forest)
				roomInfo.m_mapInfo = MapName::Sea;

			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(NOTIFY_CHANGEMAP);
			}
		}


		else if (mouseY > 277 && mouseY < 395 && mouseX > 448 && mouseX < 565) {
			if (numPlayer == MAX_PLAYER)
			{
				// ���ӽ���
				std::cout << "������ �����մϴ�" << std::endl;

				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					sendQueue[i].emplace_back(NOTIFY_START);
				}
				readyPlayer++;
				while (readyPlayer < MAX_PLAYER);

				m_isDestory = true;
				m_nextScene = SceneName::InGame;
			}
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// ������
			std::cout << "������!! �κ��!!!! " << std::endl;
			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				if (roomInfo.m_charInfo[i] == CharacterName::NONE) continue;
				sendQueue[i].emplace_back(NOTIFY_EXIT);
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