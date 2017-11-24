#include "RoomSceneGuest.h"


RoomSceneGuest::RoomSceneGuest()
{
}

RoomSceneGuest::RoomSceneGuest(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
}

RoomSceneGuest::~RoomSceneGuest()
{
}


void RoomSceneGuest::Draw(HDC hdc) {
	if (m_What_Is_Your_Map == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (m_What_Is_Your_Map == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomSceneGuest::DrawCharacter(HDC hdc) {
	int x, y;

	for (int i = 0; i < 4; ++i)
	{
		if (roomInfo.charInfo[i] == CharacterName::NONE) continue;
		x = GetXPosByIdx(i);	y = GetYPosByIdx(i);
		if (m_characterName == CharacterName::Archer)
			m_ImgArcher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (m_characterName == CharacterName::Zombie)
			m_ImgZombie.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (m_characterName == CharacterName::Knight)
			m_ImgKnight.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
		else if (m_characterName == CharacterName::Wicher)
			m_ImgWicher.TransparentBlt(hdc, x, y, 200, 500, RGB(252, 252, 252));
	}
}

int RoomSceneGuest::GetXPosByIdx(int idx)
{
	// Red ��
	if (idx == 0) return 50;
	else if (idx == 1) return 200;
	// Blue ��
	else if (idx == 2) return 600;
	else if (idx == 3) return 750;
	return -1;	// ����
}

int RoomSceneGuest::GetYPosByIdx(int idx)
{
	// Red ��
	if (idx == 0) return 90;
	else if (idx == 1) return 50;
	// Blue ��
	else if (idx == 2) return 50;
	else if (idx == 3) return 90;
	return -1;	// ����
}


bool RoomSceneGuest::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY > 80 && mouseY < 195 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "���� ��ó ������ϴ�." << std::endl;
			m_characterName = CharacterName::Archer;
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "���� ���� ������ϴ�." << std::endl;
			m_characterName = CharacterName::Wicher;

		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "���� ��� ������ϴ�." << std::endl;
			m_characterName = CharacterName::Knight;

		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "���� ���� ������ϴ�." << std::endl;
			m_characterName = CharacterName::Zombie;

		}

		else if (mouseY > 277 && mouseY < 395 && mouseX > 448 && mouseX < 565) {
			// ���ӽ���
			std::cout << "������ �����մϴ�" << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::InGame;
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// ������
			std::cout << "������!! �κ��!!!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Lobby;
		}

		return true;
	}
}

void RoomSceneGuest::Timer(const double count) {

}


void RoomSceneGuest::LoadCImage() {
	m_backImg[0].Load("Resource/Image/Room/GameRoom_guest_1.png");	//Sea
	m_backImg[1].Load("Resource/Image/Room/GameRoom_guest_2.png");	//Forest

	m_ImgArcher.Load("Resource/Image/Room/Room_Archer.png");
	m_ImgZombie.Load("Resource/Image/Room/Room_Zombie.png");
	m_ImgKnight.Load("Resource/Image/Room/Room_Knight.png");
	m_ImgWicher.Load("Resource/Image/Room/Room_Wicher.png");

}
