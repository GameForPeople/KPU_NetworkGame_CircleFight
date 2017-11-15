#include "LobbyScene.h"



LobbyScene::LobbyScene()
{
}

LobbyScene::LobbyScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
}

LobbyScene::~LobbyScene()
{
}




void LobbyScene::Draw(HDC hdc) {
	m_backImg.StretchBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 40, SRCCOPY);
}

void LobbyScene::Timer(const double count) {

}

bool LobbyScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;


	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY > 580 && mouseY < 670 && mouseX > 740 && mouseX < 845) {
			// ������
			std::cout << "�����ϴ�!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Login;
		}
		else if (mouseY > 465 && mouseY < 560 && mouseX > 740 && mouseX < 845) {
			// �����
			std::cout << "������մϴ�!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Room;
		}
		else if (mouseY > 630 && mouseY > 670 && mouseX > 645 && mouseX < 725) {
			// ä��
			std::cout << "ä���մϴ�!! " << std::endl;
		}
		return true;
	}
}



void LobbyScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Lobby/Lobby.png");
}
