#include "RoomScene.h"



RoomScene::RoomScene()
{
}

RoomScene::RoomScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
}

RoomScene::~RoomScene()
{
}


void RoomScene::Draw(HDC hdc) {
	if(m_What_Is_Your_Map == MapName::Sea)
		m_backImg[0].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);
	else if (m_What_Is_Your_Map == MapName::Forest)
		m_backImg[1].StretchBlt(hdc, 0, 0, SCREEN_WIDTH - 15, SCREEN_HEIGHT - 40, SRCCOPY);


	DrawCharacter(hdc);
}

void RoomScene::DrawCharacter(HDC hdc) {
	if (m_characterName == CharacterName::Archer)
		m_ImgArcher.TransparentBlt(hdc, 50, 50, 200,500, RGB(252, 252, 252));
	else if (m_characterName == CharacterName::Zombie)
		m_ImgZombie.TransparentBlt(hdc, 50, 50, 200,500, RGB(252, 252, 252));
	else if (m_characterName == CharacterName::Knight)
		m_ImgKnight.TransparentBlt(hdc, 50, 50, 200,500, RGB(252, 252, 252));
	else if (m_characterName == CharacterName::Wicher)
		m_ImgWicher.TransparentBlt(hdc, 50, 50, 200, 500, RGB(252, 252, 252));
}

bool RoomScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	float mouseX, mouseY;

	if (iMessage == WM_LBUTTONDOWN) {
		mouseY = HIWORD(lParam);
		mouseX = LOWORD(lParam);

		//std::cout << mouseX << "  " << mouseY << std::endl;

		if (mouseY > 80 && mouseY < 195 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 아처 골랐습니다." << std::endl;
			m_characterName = CharacterName::Archer;
		}
		else if (mouseY > 80 && mouseY < 195 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 위쳐 골랐습니다." << std::endl;
			m_characterName = CharacterName::Wicher;

		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1020 && mouseX < 1129) {
			std::cout << "나는 기사 골랐습니다." << std::endl;
			m_characterName = CharacterName::Knight;

		}
		else if (mouseY > 200 && mouseY < 320 && mouseX > 1130 && mouseX < 1270) {
			std::cout << "나는 좀비 골랐습니다." << std::endl;
			m_characterName = CharacterName::Zombie;

		}


		else if (mouseY > 440 && mouseY < 485 && mouseX > 1010 && mouseX < 1060) {
			std::cout << "나는 맵을 바꿧습니다." << std::endl;

			if (m_What_Is_Your_Map My_Map_Is MapName::Sea)
				m_What_Is_Your_Map Your_New_Map_Is MapName::Forest;
			else if (m_What_Is_Your_Map == MapName::Forest)
				m_What_Is_Your_Map = MapName::Sea;
		}

		else if (mouseY > 440 && mouseY < 485 && mouseX > 1210 && mouseX < 1250) {
			std::cout << "나는 맵을 바꿧습니다." << std::endl;

			if (m_What_Is_Your_Map == MapName::Sea)
				m_What_Is_Your_Map = MapName::Forest;
			else if (m_What_Is_Your_Map == MapName::Forest)
				m_What_Is_Your_Map = MapName::Sea;
		}


		else if (mouseY > 277  && mouseY < 395 && mouseX > 448 && mouseX < 565 ) {
			// 게임시작
			std::cout << "게임을 시작합니다" << std::endl;
			m_isDestory = true;
			m_nextScene = SceneName::InGame;
		}

		else if (mouseY > 610 && mouseY < 650 && mouseX > 1135 && mouseX < 1246) {
			// 나가기
			std::cout << "나가요!! 로비로!!!! " << std::endl;
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
