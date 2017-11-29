#include "LobbyScene.h"



LobbyScene::LobbyScene()
{
}

LobbyScene::LobbyScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();

	m_network->ChageSceneName(SceneName::Lobby);

}

LobbyScene::LobbyScene(HWND hWnd, Network& network) : Scene(hWnd)
{
	LoadCImage();
	m_network = &network;
	m_network->ChageSceneName(SceneName::Lobby);
}

LobbyScene::LobbyScene(HWND hWnd, Network* network) : Scene(hWnd)
{
	LoadCImage();
	m_network = network;
	m_network->ChageSceneName(SceneName::Lobby);
}

LobbyScene::~LobbyScene()
{
}


void LobbyScene::Draw(HDC hdc) {
	m_backImg.StretchBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT - 40, SRCCOPY);

	int x = 45, y = 100;
	for (int i = 0; i < MAX_ROOM_COUNT; i++) {
		
		if (m_roomData[i].playersNumber) {
			if (m_roomData[i].mapNumber == 1) {
				m_roomImg[0].TransparentBlt(hdc, x, y, 270, 170, RGB(255, 255, 255));
			}
			else if (m_roomData[i].mapNumber == 2) {
				m_roomImg[0].TransparentBlt(hdc, x, y, 270, 170, RGB(255, 255, 255));
			}
		}
		x += 300;
		if (i == 3) {
			x = 45;
			y = 280;
		}
	}

	HFONT hFont = CreateFont(30, 0, 0, 0, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0,
		VARIABLE_PITCH | FF_ROMAN, TEXT("�ü�"));

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


	SetTextColor(hdc, RGB(30, 30, 30));
	TextOut(hdc, 900, 500, m_network->myData.id, 4);
	
	SetTextColor(hdc, RGB(30, 30, 230));

	TCHAR strWin[10]{};
	sprintf(strWin, TEXT(". %d"), m_network->myData.winCount);
	TextOut(hdc, 950, 600, strWin, lstrlen(strWin));

	SetTextColor(hdc, RGB(230, 30, 30));

	TCHAR strLose[10]{};
	sprintf(strLose, TEXT(". %d"), m_network->myData.loseCount);
	TextOut(hdc, 1140, 600, strLose, lstrlen(strLose));

	//std::cout << m_network->myData.winCount << "  " << m_network->myData.loseCount << std::endl;
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);
}

void LobbyScene::Timer(const double count) {
	//_sleep(100);

	if (m_network->GetRecvType() == 2) {
		EnterCriticalSection(&m_network->LOBBY_UPDATE_SECTION);

		::memcpy(m_chatBuf[0], m_network->m_permitChat->chat[0], sizeof(m_network->m_permitChat->chat[0]));
		::memcpy(m_chatBuf[1], m_network->m_permitChat->chat[1], sizeof(m_network->m_permitChat->chat[1]));
		::memcpy(m_chatBuf[2], m_network->m_permitChat->chat[2], sizeof(m_network->m_permitChat->chat[2]));
		::memcpy(m_chatBuf[3], m_network->m_permitChat->chat[3], sizeof(m_network->m_permitChat->chat[3]));
		::memcpy(m_chatBuf[4], m_network->m_permitChat->chat[4], sizeof(m_network->m_permitChat->chat[4]));

		for (int i = 0; i < MAX_ROOM_COUNT; i++) {
			m_roomData[i].mapNumber = m_network->m_updateLobbyInfo->m_mapNumber[i];
			m_roomData[i].playersNumber = m_network->m_updateLobbyInfo->m_playersNumber[i];
		}

		m_network->SetRecvType(0);

		LeaveCriticalSection(&m_network->LOBBY_UPDATE_SECTION);
	}
}

bool LobbyScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		std::cout << mouseX << "  " << mouseY << std::endl;

		if (mouseY > 580 && mouseY < 670 && mouseX > 740 && mouseX < 845) {
			// ������
			std::cout << "�����ϴ�!! " << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::Login;
		}
		else if (mouseY > 465 && mouseY < 560 && mouseX > 740 && mouseX < 845) {
			// �����
			std::cout << "������մϴ�!! " << std::endl;
			m_network->SetSendType(DEMAND_CREATEROOM);
			
			while (7) {
				std::cout << ".";
				_sleep(100);
				if (m_network->GetRecvType())
					break;
			}

			if (m_network->GetRecvType() == PERMIT_CREATEROOM) {
				//m_network->m_permitCreateRoom->roomIndex;
				m_isDestory = true;
				m_nextScene = SceneName::Room;
			}
		}
		else if (mouseY > 630 && mouseY < 670 && mouseX > 640 && mouseX < 725) {
			EnterCriticalSection(&m_network->LOBBY_UPDATE_SECTION);

			m_network->m_demandChat = new DemandChatStruct;

			::memcpy(m_network->m_demandChat->chat, m_chat, sizeof(m_chat));
			
			//std::cout << "m_chat :" << m_chat <<std::endl;
			//std::cout << "demandchat :" << m_network.m_demandChat->chat << std::endl;

			//EnterCriticalSection(&m_network.SEND_SECTION);
			m_network->SetSendType(DEMAND_CHAT);


			while (7) {
				std::cout << ".";
				_sleep(100);
				if (m_network->GetRecvType())
					break;
			}

			m_Len = 0;
			m_chat[0] = '\0';

			::memcpy(m_chatBuf[0], m_network->m_permitChat->chat[0], sizeof(m_network->m_permitChat->chat[0]));
			::memcpy(m_chatBuf[1], m_network->m_permitChat->chat[1], sizeof(m_network->m_permitChat->chat[1]));
			::memcpy(m_chatBuf[2], m_network->m_permitChat->chat[2], sizeof(m_network->m_permitChat->chat[2]));
			::memcpy(m_chatBuf[3], m_network->m_permitChat->chat[3], sizeof(m_network->m_permitChat->chat[3]));
			::memcpy(m_chatBuf[4], m_network->m_permitChat->chat[4], sizeof(m_network->m_permitChat->chat[4]));

			m_network->SetRecvType(0);

			LeaveCriticalSection(&m_network->LOBBY_UPDATE_SECTION);
			//memcpy(m_chatBuf[0], m_chatBuf[1], sizeof(m_chatBuf[1]));
			//memcpy(m_chatBuf[1], m_chatBuf[2], sizeof(m_chatBuf[2]));
			//memcpy(m_chatBuf[2], m_chatBuf[3], sizeof(m_chatBuf[3]));
			//memcpy(m_chatBuf[3], m_chatBuf[4], sizeof(m_chatBuf[4]));
			//memcpy(m_chatBuf[4], m_chat, sizeof(m_chat));
			//
			//m_Len = 0;
			//m_chat[0] = '\0';
		}
		return true;
	}
}

bool LobbyScene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	switch (iMessage) {
	case WM_KEYDOWN:
			if ('A' <= wParam && wParam <= 'z') {
				if (m_Len < BUF_SIZE - 1) {
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
				if (m_Len <BUF_SIZE - 2) {
					m_chat[m_Len++] = ' ';
					m_chat[m_Len] = '\0';
				}
			}
			else if (wParam == VK_RETURN) {
				EnterCriticalSection(&m_network->LOBBY_UPDATE_SECTION);

				//std::cout << "1" << std::endl;
				m_network->m_demandChat = new DemandChatStruct;
				//std::cout << "2" << std::endl;

				::memcpy(m_network->m_demandChat->chat, m_chat, sizeof(m_chat));
				//std::cout << "3" << std::endl;

				//EnterCriticalSection(&m_network->SEND_SECTION);
				m_network->SetSendType(DEMAND_CHAT);
				//LeaveCriticalSection(&m_network->SEND_SECTION);
				//std::cout << "4" << std::endl;
				while (7) {
					std::cout << ".";
					_sleep(100);
					if (m_network->GetRecvType())
						break;
				}

				m_Len = 0;
				m_chat[0] = '\0';

				::memcpy(m_chatBuf[0], m_network->m_permitChat->chat[0], sizeof(m_network->m_permitChat->chat[0]));
				::memcpy(m_chatBuf[1], m_network->m_permitChat->chat[1], sizeof(m_network->m_permitChat->chat[1]));
				::memcpy(m_chatBuf[2], m_network->m_permitChat->chat[2], sizeof(m_network->m_permitChat->chat[2]));
				::memcpy(m_chatBuf[3], m_network->m_permitChat->chat[3], sizeof(m_network->m_permitChat->chat[3]));
				::memcpy(m_chatBuf[4], m_network->m_permitChat->chat[4], sizeof(m_network->m_permitChat->chat[4]));

				m_network->SetRecvType(0);

				LeaveCriticalSection(&m_network->LOBBY_UPDATE_SECTION);
			}
		}

	return true;
}

void LobbyScene::LoadCImage() {
	m_backImg.Load("Resource/Image/Lobby/Lobby.png");
	m_roomImg[0].Load("Resource/Image/Lobby/RoomUI_Sea_Wait.png");
	m_roomImg[1].Load("Resource/Image/Lobby/RoomUI_Sea_Play.png");
	m_roomImg[2].Load("Resource/Image/Lobby/RoomUI_Forest_Wait.png");
	m_roomImg[3].Load("Resource/Image/Lobby/RoomUI_Forest_Play.png");
}
