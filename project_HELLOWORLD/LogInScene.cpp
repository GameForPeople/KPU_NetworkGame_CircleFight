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
	
	if (m_isDrawUI) {
		if(m_userInsertType)
			m_uiImg[1].TransparentBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(255, 255, 255));
		else
			m_uiImg[0].TransparentBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, RGB(255, 255, 255));
	}


	HFONT hFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
	                          VARIABLE_PITCH | FF_ROMAN, TEXT("궁서"));
	
	HFONT OldFont = (HFONT)SelectObject(hdc, hFont);

	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(255, 255, 255));

	TextOut(hdc, 466, 440, m_id, lstrlen(m_id));
	TextOut(hdc, 466, 520, m_pw, lstrlen(m_pw));


	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
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
		else if (mouseY > 430 && mouseY < 500 && mouseX > 450 && mouseX < 825) {
			//550 750 630
			std::cout << "아이디입력합니다!! " << std::endl;
			m_userInsertType = 1;

		}
		else if (mouseY > 505 && mouseY < 580 && mouseX > 450 && mouseX < 825) {
			//550 750 630
			std::cout << "비밀번호입력합니다!! " << std::endl;
			m_userInsertType = 2;
		}
			return true;
	}
}

bool LoginScene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {

	switch (iMessage) {
	case WM_KEYDOWN:
		if (m_userInsertType == 1) {
			if ('A' <= wParam && wParam <= 'z') {
				if (m_idLen < 4) {
					m_id[m_idLen++] = (TCHAR)wParam;
					std::cout << m_id << std::endl;
					m_id[m_idLen] = '\0';
				}
			}
			else if (wParam == VK_BACK) {
				if (m_idLen >= 1) {
					m_id[m_idLen - 1] = '\0';
					m_idLen--;
				}
			}
			else if (wParam == VK_TAB) {
				m_userInsertType = 2;
			}
		}
		else if (m_userInsertType == 2) {
			if ('0' <= wParam && wParam <= '9') {
				if (m_pwLen < 4) {
					m_pw[m_pwLen++] = (TCHAR)wParam;
					std::cout << m_pw << std::endl;
					m_pw[m_pwLen] = '\0';
				}
			}
			else if (wParam == VK_BACK) {
				if (m_pwLen >= 1) {
					m_pw[m_pwLen - 1] = '\0';
					m_pwLen--;
				}
			}
		}
	break;
	}

	return true;
}




void LoginScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Login/backImg.png");

	m_logoImg.Load("Resource/Image/Login/LogoImg.png");
	m_pawnImg.Load("Resource/Image/Login/PawnImg.png");
	m_uiImg[0].Load("Resource/Image/Login/UIImg.png");
	m_uiImg[1].Load("Resource/Image/Login/UIImg_2.png");
}
