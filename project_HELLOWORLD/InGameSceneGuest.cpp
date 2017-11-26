#include "InGameSceneGuest.h"
#include <fstream>

#define TO_DEBUG_ARCHER
//#define TO_DEBUG_ZOMBIE
//#define TO_DEBUG_KNIGHT
//#define TO_DEBUG_WICHER

//#define TO_DEBUG_MAP_SEA
#define TO_DEBUG_MAP_FOREST


InGameSceneGuest::InGameSceneGuest(HWND hwnd) : Scene(hwnd)
{
#ifdef TO_DEBUG_ARCHER
	m_characterArr[0] = Pawn(CharacterName::Archer);
#endif

#ifdef TO_DEBUG_ZOMBIE
	m_characterArr[0] = Pawn(CharacterName::Zombie);
#endif

#ifdef TO_DEBUG_KNIGHT
	m_characterArr[0] = Pawn(CharacterName::Knight);
#endif

#ifdef TO_DEBUG_WICHER
	m_characterArr[0] = Pawn(CharacterName::Wicher);
#endif

	m_characterArr[0].SetState(State::Fall);

#ifdef TO_DEBUG_MAP_SEA
	m_map = new Map(0, 0, "Resource/Image/Background/Background.png");
#endif 

#ifdef TO_DEBUG_MAP_FOREST
	m_map = new Map(0, 0, "Resource/Image/Background/Background_2.png");
#endif 
	m_platImg[0] = new CImage;
	m_platImg[1] = new CImage;

	m_platImg[0]->Load("Resource/Image/Plat/Plat_2.png");
	m_platImg[1]->Load("Resource/Image/Plat/Plat_1.png");

	m_numPlat = PLAT_MAX_NUMBER;
	m_platArr = new BaseObject[m_numPlat];
	LoadPlat();

	m_inGameUI = new InGameSceneUI;
}

InGameSceneGuest::InGameSceneGuest(HWND hwnd, MapName insertMap, CharacterName* insertCharacter) : Scene(hwnd)
{
	sendQueueGuest.clear();
	memset(m_emotionNumber, 0, sizeof(m_emotionNumber));
	memset(&basicInfo, 0, sizeof(basicInfo));

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (insertCharacter[i] == CharacterName::Archer)
			m_characterArr[i] = Pawn(CharacterName::Archer);
		else if (insertCharacter[i] == CharacterName::Zombie)
			m_characterArr[i] = Pawn(CharacterName::Zombie);
		else if (insertCharacter[i] == CharacterName::Knight)
			m_characterArr[i] = Pawn(CharacterName::Knight);
		else if (insertCharacter[i] == CharacterName::Wicher)
			m_characterArr[i] = Pawn(CharacterName::Wicher);
	}


	m_characterArr->SetState(State::Fall);

	if (insertMap == MapName::Sea)
		m_map = new Map(0, 0, "Resource/Image/Background/Background.png");
	else if (insertMap == MapName::Forest)
		m_map = new Map(0, 0, "Resource/Image/Background/Background_2.png");

	m_platImg[0] = new CImage;
	m_platImg[1] = new CImage;

	m_platImg[0]->Load("Resource/Image/Plat/Plat_2.png");
	m_platImg[1]->Load("Resource/Image/Plat/Plat_1.png");

	m_numPlat = PLAT_MAX_NUMBER;
	m_platArr = new BaseObject[m_numPlat];
	LoadPlat();

	m_inGameUI = new InGameSceneUI;

	sendQueueGuest.push_back(NOTIFYSTART);
}

InGameSceneGuest::InGameSceneGuest()
{
}

InGameSceneGuest::~InGameSceneGuest()
{
}



void InGameSceneGuest::Draw(HDC hdc) {
	m_map->Draw(hdc);

	for (int i = 0; i < m_numPlat; i++) {
		if (m_platArr[i].GetPos().y != PLAT_LOW_HEIGHT)
			m_platImg[0]->TransparentBlt(hdc, m_platArr[i].GetPos().x, m_platArr[i].GetPos().y, PLAT_WIDTH, PLAT_HEIGHT, RGB(255, 255, 255));
		else
			m_platImg[1]->TransparentBlt(hdc, m_platArr[i].GetPos().x, m_platArr[i].GetPos().y, PLAT_WIDTH, m_platImg[1]->GetHeight(), RGB(255, 255, 255));
	}

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (i == m_idx) continue;
		m_characterArr[i].Draw(hdc, m_characterArr[i].GetTotalDistance() - m_characterArr[m_idx].GetTotalDistance(), m_characterArr[i].GetState());
	}
	m_characterArr[m_idx].Draw(hdc, m_characterArr[m_idx].GetState());

	m_inGameUI->DrawComboUI(hdc, m_characterArr[m_idx].GetCombo());
	m_inGameUI->DrawBarUI(hdc, m_characterArr[m_idx].GetTotalDistance() / 100);
	m_inGameUI->DrawInventoryUI(hdc, 0, 0);

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (m_emotionNumber[i])
			m_inGameUI->DrawEmotionUI(hdc, m_emotionNumber[i], m_characterArr[i].GetPos().x, m_characterArr[i].GetPos().y);
		else
			m_inGameUI->DrawHeadUpUI(hdc, m_characterArr[i].GetPos().y);
	}


}

void InGameSceneGuest::Timer(const double time) {
	m_map->Update(basicInfo.speed[m_idx], basicInfo.eTime + time);
	/*for (int i = 0; i < PLAT_MAX_NUMBER; i++)
		m_platArr[i].Update(basicInfo.speed[m_idx], basicInfo.eTime + time);*/
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		m_characterArr[i].ComputeTotalDistance(basicInfo.speed[i]);
		m_characterArr[i].SetPos(basicInfo.position[i]);
		m_characterArr[i].SetState(basicInfo.state[i]);
		m_characterArr[i].GetUnit().SetImageCount(basicInfo.imgCnt[i]);
		m_characterArr[i].SetCombo(basicInfo.combo[i]);
		m_emotionNumber[i] = basicInfo.emoticon[i];
	}
	//ShowPawnState();	//Debug
}

bool InGameSceneGuest::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//if (m_characterArr->InsertKey(wParam)) return true;
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		//if (m_characterArr[0].InsertKey(wParam))return true;

		switch (wParam) {
		case VK_SPACE:

			break;
		case 'G':

			break;
		case '1':
			m_emotionNumber[m_idx] = 1;
			break;
		case '2':
			m_emotionNumber[m_idx] = 2;
			break;
		case '3':
			m_emotionNumber[m_idx] = 3;
			break;
		case '4':
			m_emotionNumber[m_idx] = 4;
			break;
		case '5':
			m_emotionNumber[m_idx] = 5;
			break;
		case '6':
			m_emotionNumber[m_idx] = 6;

			break;
		}
	}
	case WM_KEYUP:
	{
		switch (wParam)
		{
			break;
		}
		return false;
	}
	}
}

bool InGameSceneGuest::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	return true;
}

void InGameSceneGuest::Destory() {

}



void InGameSceneGuest::LoadPlat() {
	//ifstream inFile("Resource/Data/platData.txt", ios::in);
	ifstream inFile("Resource/Data/NewPlatData.txt", ios::in);
	//to debug
	//ifstream inFile("platData.txt", ios::in);	

	int posX = 1;
	int posY = 0;

	for (int i = 0; i < m_numPlat; i++) {

		inFile >> posY;

		m_platArr[i].SetPos(posX * i *  PLAT_WIDTH, (posY * (-100)) + PLAT_LOW_HEIGHT);

		//to debug
		//std::cout << i << "¹øÂ°  " << posY << "   " << m_platArr[i].GetPos().x << "   " << m_platArr[i].GetPos().y << std::endl;
	}
	inFile.close();
	/*
	FILE *fp;

	fp = fopen("Resource/Data/platData.txt", "r");

	int posX = 1;
	int posY = 0;

	for(int i = 0; i < m_numPlat; i++){
	fscanf(fp, "%d", &posY);
	m_platArr[i].SetPos(posX * i * 100, (posY * (-100)) + 600);
	std::cout << posY << std::endl;
	}

	std::fclose(fp);
	*/
}

void InGameSceneGuest::ShowPawnState() {

	if (m_characterArr[0].GetState() == State::Run) {
		std::cout << "Run" << std::endl;
	}
	else if (m_characterArr[0].GetState() == State::JumpLoop) {
		std::cout << "JUMPLOOP" << std::endl;
	}
	else if (m_characterArr[0].GetState() == State::JumpEnd) {
		std::cout << "JUMPEnd" << std::endl;
	}
	else if (m_characterArr[0].GetState() == State::DoubleJumpEnd) {
		std::cout << "DoubleJUMPEnd" << std::endl;
	}

	else if (m_characterArr[0].GetState() == State::Fall) {
		std::cout << "Fall" << std::endl;
	}
	else {
		if (m_characterArr[0].GetState() == State::Boost) {
			std::cout << "Boost" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::Collide) {
			std::cout << "Collide" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::Death) {
			std::cout << "Death" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::DoubleJumpLoop) {
			std::cout << "DoupleJump" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::Sleep) {
			std::cout << "Sleep" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::Slow) {
			std::cout << "Slow" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::Stun) {
			std::cout << "Stun" << std::endl;
		}
		else if (m_characterArr[0].GetState() == State::TripleJump) {
			std::cout << "TripleJump" << std::endl;
		}
		else
			std::cout << "What is the Problem!!" << std::endl;
	}
}