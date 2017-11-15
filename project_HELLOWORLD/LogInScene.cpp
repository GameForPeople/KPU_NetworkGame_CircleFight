#include "LoginScene.h"



LoginScene::LoginScene()
{
}

LoginScene::LoginScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
}

LoginScene::~LoginScene()
{
}


void LoginScene::Draw(HDC hdc) {
	m_backImg.BitBlt(hdc, 0, 0, SRCCOPY);
	
	m_logoImg.TransparentBlt(hdc, 0, 0 - 400 + m_logoTimer, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(255, 255, 255));
	
	if(m_isDrawPawn)
		m_pawnImg.TransparentBlt(hdc, 0, 0, m_pawnTimer, SCREEN_HEIGHT, RGB(255, 255, 255));
	
	if(m_isDrawUI)
		m_uiImg.TransparentBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(255, 255, 255));
}

void LoginScene::Timer(const double count) {
	m_logoTimer+= 3;

	if (m_logoTimer >= 401)
		m_logoTimer = 400;
	else if (m_logoTimer >= 395)
		m_isDrawPawn = true;

	if (m_isDrawPawn) {
		m_pawnTimer += 20;
	
		if (m_pawnTimer > SCREEN_WIDTH)
			m_pawnTimer = SCREEN_WIDTH;
		else if (m_pawnTimer > SCREEN_WIDTH - 15)
			m_uiOnoff = true;
	}
	
	if (m_uiOnoff) {
		m_uiTimer++;
		
		if (m_uiTimer < 50) {
			if (m_uiTimer % 15 < 8) {
				m_isDrawUI = true;
			}
			else if (m_uiTimer % 15 >= 8) {
				m_isDrawUI = false;
			}
		}
		else m_isDrawUI = true;
	}
}

bool LoginScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;
	
	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		if (mouseY < 100 && mouseX > 1130) {
			// Sign In
			std::cout << "회원가입합니다!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Lobby;
		}
		else if (mouseY > 600 && mouseX > 550 && mouseX < 750 ) {
			//550 750 630
			std::cout << "로그인입니다!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Lobby;

		}
			return true;
	}
}




void LoginScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Login/backImg.png");
	m_logoImg.Load("Resource/Image/Login/LogoImg.png");
	m_pawnImg.Load("Resource/Image/Login/PawnImg.png");
	m_uiImg.Load("Resource/Image/Login/UIImg.png");
}
