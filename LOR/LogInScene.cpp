#include "LoginScene.h"



LoginScene::LoginScene()
{
}



LoginScene::LoginScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
	m_network = new Network;
	m_network->ChageSceneName(SceneName::Login);

	//ThreadStruct threadArgument;
	//printf("  %p  \n", m_network);
	//threadArgument.network = m_network;
	//printf("  %p  \n", threadArgument.network);
	//HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, &threadArgument, 0, NULL);
	//printf("  %p  \n", m_network);
	
	//HANDLE hThread = CreateThread(NULL, 0, ThreadFunction, &m_network, 0, NULL);
	//
	//if (hThread == NULL) { closesocket(m_network->GetSocket()); }
	//else { CloseHandle(hThread); }
}

LoginScene::LoginScene(HWND hWnd, Network* network) : Scene(hWnd)
{
	m_network = network;

	//BGM재생
	m_network->m_nowBgmNumber = 0;
	m_network->m_system->playSound(FMOD_CHANNEL_REUSE, m_network->m_sound[0], false, &(m_network->m_channel[0]));

	//효과음 재생
	//PlaySound("Resource\\sound\\디딩.wav", NULL, SND_ASYNC);
	//m_network.Connect(m_network);
	//m_network->Connect();
	m_network->ChageSceneName(SceneName::Login);
	//m_network.ChageSceneName(SceneName::Login);
	LoadCImage();
}

LoginScene::~LoginScene()
{
	//delete m_network;
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
			if (m_idLen == 4 && m_pwLen == 4) {
			// Sign In
				std::cout << "회원가입합니다!! " << std::endl;
				m_network->m_demandLogin = new DemandLoginStruct;

				m_network->m_demandLogin->type = 2;
				memcpy(m_network->m_demandLogin->ID, m_id, sizeof(m_id));
				m_network->m_demandLogin->PW = ChangeNumberCharToInt(m_pw);

				m_network->SetSendType(DEMAND_LOGIN);

				while (7) {
					std::cout << ".";
					_sleep(100);
					if (m_network->GetRecvType())
						break;
				}

				if (m_network->GetRecvType() == FAIL_LOGIN)
					return true;
				else if (m_network->GetRecvType() == PERMIT_LOGIN) {
					memcpy(m_network->myData.id, m_network->m_demandLogin->ID, sizeof(5));
					m_network->myData.pw = m_network->m_demandLogin->PW;
					m_network->myData.winCount = m_network->m_permitLogin->winCount;
					m_network->myData.loseCount = m_network->m_permitLogin->loseCount;

					m_isDestory = true;
					m_nextScene = SceneName::Lobby;
					m_network->SetRecvType();
					m_network->ChageSceneName(SceneName::Lobby);

					delete (m_network->m_demandLogin);
					delete (m_network->m_permitLogin);

					return true;
				}
			}
		}
		else if (mouseY > 600 && mouseX > 550 && mouseX < 750) {
			if (m_idLen == 4 && m_pwLen == 4) {
				std::cout << "로그인합니다!! " << std::endl;

				if (m_network->m_demandLogin == NULL)
					m_network->m_demandLogin = new DemandLoginStruct;

				m_network->m_demandLogin->type = 1;
				memcpy(m_network->m_demandLogin->ID, m_id, sizeof(m_id));
				m_network->m_demandLogin->PW = ChangeNumberCharToInt(m_pw);

				//std::cout << "1.로그인 또는  회원가입 정보 보내요!" << std::endl;
				//std::cout << "1.보내는 타입은  " << m_network->m_demandLogin->type << std::endl;
				std::cout << "	1.입력된 ID는  " << m_network->m_demandLogin->ID << std::endl;
				std::cout << "	2.입력된 PW는  " << m_network->m_demandLogin->PW << std::endl;

				m_network->SetSendType(DEMAND_LOGIN);

				while (7) {
					std::cout << ".";
					_sleep(100);
					if (m_network->GetRecvType())
						break;
				}

				//std::cout << "받은 번호는 " << m_network->GetRecvType() << std::endl;

				if (m_network->GetRecvType() == FAIL_LOGIN)
					return true;
				else if (m_network->GetRecvType() == PERMIT_LOGIN) {
					memcpy(m_network->myData.id, m_network->m_demandLogin->ID, sizeof(5));
					m_network->myData.pw = m_network->m_demandLogin->PW;
					m_network->myData.winCount = m_network->m_permitLogin->winCount;
					m_network->myData.loseCount = m_network->m_permitLogin->loseCount;

					m_isDestory = true;
					m_nextScene = SceneName::Lobby;
					m_network->SetRecvType();
					m_network->ChageSceneName(SceneName::Lobby);

					delete (m_network->m_demandLogin);
					delete (m_network->m_permitLogin);

					return true;
				}
			}
		}
		else if (mouseY > 430 && mouseY < 500 && mouseX > 450 && mouseX < 825) {
			//550 750 630
			std::cout << "	아이디를 입력합니다!! " << std::endl;
			m_userInsertType = 1;

		}
		else if (mouseY > 505 && mouseY < 580 && mouseX > 450 && mouseX < 825) {
			//550 750 630
			std::cout << "	비밀번호를 입력합니다!! " << std::endl;
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
					//std::cout << m_id << std::endl;
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
					//std::cout << m_pw << std::endl;
					m_pw[m_pwLen] = '\0';
				}
			}
			else if (wParam == VK_BACK) {
				if (m_pwLen >= 1) {
					m_pw[m_pwLen - 1] = '\0';
					m_pwLen--;
				}
			}
			else if (wParam == VK_RETURN) {
				if (m_idLen == 4 && m_pwLen == 4) {
					std::cout << "	로그인합니다!! " << std::endl;

					if (m_network->m_demandLogin == NULL)
						m_network->m_demandLogin = new DemandLoginStruct;

					m_network->m_demandLogin->type = 1;
					memcpy(m_network->m_demandLogin->ID, m_id, sizeof(m_id));
					m_network->m_demandLogin->PW = ChangeNumberCharToInt(m_pw);

					//std::cout << "1.로그인 또는  회원가입 정보 보내요!" << std::endl;
					//std::cout << "1.보내는 타입은  " << m_network->m_demandLogin->type << std::endl;
					std::cout << "	1.입력된 ID는  " << m_network->m_demandLogin->ID << std::endl;
					std::cout << "	2.입력된 PW는  " << m_network->m_demandLogin->PW << std::endl;

					m_network->SetSendType(DEMAND_LOGIN);

					while (7) {
						std::cout << ".";
						_sleep(100);
						if (m_network->GetRecvType())
							break;
					}

					//std::cout << "받은 번호는 " << m_network->GetRecvType() << std::endl;

					if (m_network->GetRecvType() == FAIL_LOGIN)
						return true;
					else if (m_network->GetRecvType() == PERMIT_LOGIN) {
						memcpy(m_network->myData.id, m_network->m_demandLogin->ID, sizeof(5));
						m_network->myData.pw = m_network->m_demandLogin->PW;
						m_network->myData.winCount = m_network->m_permitLogin->winCount;
						m_network->myData.loseCount = m_network->m_permitLogin->loseCount;

						m_isDestory = true;
						m_nextScene = SceneName::Lobby;
						m_network->SetRecvType();
						m_network->ChageSceneName(SceneName::Lobby);

						delete (m_network->m_demandLogin);
						delete (m_network->m_permitLogin);

						return true;
					}
				}
			}
		}
	break;
	}

	return true;
}

int LoginScene::ChangeNumberCharToInt(TCHAR* oldPw) {
	int pwBuffer{};
	for (int i = 0; i < 4; i++) {
		int numberBuffer{};
		if (oldPw[i] == '0') numberBuffer = 0;
		else if (oldPw[i] == '1') numberBuffer = 1;
		else if (oldPw[i] == '2') numberBuffer = 2;
		else if (oldPw[i] == '3') numberBuffer = 3;
		else if (oldPw[i] == '4') numberBuffer = 4;
		else if (oldPw[i] == '5') numberBuffer = 5;
		else if (oldPw[i] == '6') numberBuffer = 6;
		else if (oldPw[i] == '7') numberBuffer = 7;
		else if (oldPw[i] == '8') numberBuffer = 8;
		else if (oldPw[i] == '9') numberBuffer = 9;

		if(i == 0)	pwBuffer += numberBuffer * 1000;
		else if (i == 1) pwBuffer += numberBuffer * 100;
		else if (i == 2) pwBuffer += numberBuffer * 10;
		else if (i == 3) pwBuffer += numberBuffer;
	}
	return pwBuffer;
}

void LoginScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Login/backImg.png");

	m_logoImg.Load("Resource/Image/Login/LogoImg.png");
	m_pawnImg.Load("Resource/Image/Login/PawnImg.png");
	m_uiImg[0].Load("Resource/Image/Login/UIImg.png");
	m_uiImg[1].Load("Resource/Image/Login/UIImg_2.png");
}
