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



	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));

	HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(15, 15, 15));

	TextOut(hdc, 20, 640, m_chat, lstrlen(m_chat));

	SetTextColor(hdc, RGB(230, 230, 230));

	TextOut(hdc, 20, 470, m_chatBuf[0], lstrlen(m_chatBuf[0]));
	TextOut(hdc, 20, 500, m_chatBuf[1], lstrlen(m_chatBuf[1]));
	TextOut(hdc, 20, 530, m_chatBuf[2], lstrlen(m_chatBuf[2]));
	TextOut(hdc, 20, 560, m_chatBuf[3], lstrlen(m_chatBuf[3]));
	TextOut(hdc, 20, 590, m_chatBuf[4], lstrlen(m_chatBuf[4]));

	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

}

void LobbyScene::Timer(const double count) {

}

bool LobbyScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;


	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		std::cout << mouseX << "  " << mouseY << std::endl;

		if (mouseY > 580 && mouseY < 670 && mouseX > 740 && mouseX < 845) {
			// 나가기
			std::cout << "나갑니다!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Login;
		}
		else if (mouseY > 465 && mouseY < 560 && mouseX > 740 && mouseX < 845) {
			// 방생성
			std::cout << "방생성합니다!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Room;
		}
		else if (mouseY > 630 && mouseY < 670 && mouseX > 640 && mouseX < 725) {
			memcpy(m_chatBuf[0], m_chatBuf[1], sizeof(m_chatBuf[1]));
			memcpy(m_chatBuf[1], m_chatBuf[2], sizeof(m_chatBuf[2]));
			memcpy(m_chatBuf[2], m_chatBuf[3], sizeof(m_chatBuf[3]));
			memcpy(m_chatBuf[3], m_chatBuf[4], sizeof(m_chatBuf[4]));
			memcpy(m_chatBuf[4], m_chat, sizeof(m_chat));

			m_Len = 0;
			m_chat[0] = '\0';
		}
		return true;
	}
}

bool LobbyScene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_KEYDOWN:
			if ('A' <= wParam && wParam <= 'z') {
				if (m_Len < 99) {
					m_chat[m_Len++] = (TCHAR)wParam;
					m_chat[m_Len] = '\0';
				}
			}
			else if (wParam == VK_BACK) {
				if (m_Len >= 1) {
					m_chat[m_Len - 1] = '\0';
					m_Len--;
				}
			}
			else if (wParam == VK_SPACE) {
				if (m_Len < 98) {
					m_chat[m_Len++] = ' ';
					m_chat[m_Len] = '\0';
				}
			}
			else if (wParam == VK_RETURN) {
				memcpy(m_chatBuf[0], m_chatBuf[1], sizeof(m_chatBuf[1]));
				memcpy(m_chatBuf[1], m_chatBuf[2], sizeof(m_chatBuf[2]));
				memcpy(m_chatBuf[2], m_chatBuf[3], sizeof(m_chatBuf[3]));
				memcpy(m_chatBuf[3], m_chatBuf[4], sizeof(m_chatBuf[4]));
				memcpy(m_chatBuf[4], m_chat, sizeof(m_chat));

				m_Len = 0;
				m_chat[0] = '\0';
			}
		}

	return true;
}

void LobbyScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Lobby/Lobby.png");
}
