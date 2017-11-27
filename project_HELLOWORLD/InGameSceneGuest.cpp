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

	emotionNum = m_emotionNumber;

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
	
	for (int i = basicInfo.platInfo.idx; i < basicInfo.platInfo.idx + PLAT_SHOWN_CNT; i++) {
		if (m_platArr[i].GetPos().y != PLAT_LOW_HEIGHT)
			m_platImg[0]->TransparentBlt(hdc, m_platArr[i].GetPos().x, m_platArr[i].GetPos().y, PLAT_WIDTH, PLAT_HEIGHT, RGB(255, 255, 255));
		else
			m_platImg[1]->TransparentBlt(hdc, m_platArr[i].GetPos().x, m_platArr[i].GetPos().y, PLAT_WIDTH, m_platImg[1]->GetHeight(), RGB(255, 255, 255));
	}

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (i == m_idx) continue;
		//m_characterArr[i].NetworkDrawCharacter(hdc, basicInfo.totalDis[m_idx], basicInfo.totalDis[i], basicInfo.position[i].y,
		//	basicInfo.imgCnt[i], basicInfo.state[i]);
		m_characterArr[i].Draw(hdc, basicInfo.totalDis[i] - basicInfo.totalDis[m_idx], basicInfo.state[i]);
	}
	//m_characterArr[m_idx].Draw(hdc, basicInfo.state[m_idx]);
	m_characterArr[m_idx].NetworkDrawCharacter(hdc, 0, 0, basicInfo.position[m_idx].y,
		basicInfo.imgCnt[m_idx], basicInfo.state[m_idx]);

	m_inGameUI->DrawComboUI(hdc, basicInfo.combo[m_idx]);
	m_inGameUI->DrawBarUI(hdc, m_characterArr[m_idx].GetTotalDistance() / 100);
	m_inGameUI->DrawInventoryUI(hdc, 0, 0);

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (basicInfo.emoticon[i])
			m_inGameUI->DrawEmotionUI(hdc, basicInfo.emoticon[i], basicInfo.position[i].x, basicInfo.position[i].y);
		else
			m_inGameUI->DrawHeadUpUI(hdc, basicInfo.position[i].y);
	}


}

void InGameSceneGuest::Timer(const double time) {
	m_map->Update(basicInfo.speed[m_idx], time);

	m_platArr[basicInfo.platInfo.idx].SetXPos(basicInfo.platInfo.xPos);
	for (int i = basicInfo.platInfo.idx + 1, j=1; i < basicInfo.platInfo.idx + PLAT_SHOWN_CNT; ++i, ++j)
	{
		m_platArr[i].SetXPos(basicInfo.platInfo.xPos + PLAT_WIDTH * j);
	}

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		m_characterArr[i].SetPos(basicInfo.position[i]);
		m_characterArr[i].GetUnit().SetImageCount(basicInfo.imgCnt[i]);
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
			sendQueueGuest.emplace_back(INPUT_JUMP);
			break;
		case 'G':

			break;
		case '1':
			m_emotionNumber[m_idx] = 1;
			sendQueueGuest.emplace_back(INPUT_EMOTION);
			break;
		case '2':
			m_emotionNumber[m_idx] = 2;
			sendQueueGuest.emplace_back(INPUT_EMOTION);
			break;
		case '3':
			m_emotionNumber[m_idx] = 3;
			sendQueueGuest.emplace_back(INPUT_EMOTION);
			break;
		case '4':
			m_emotionNumber[m_idx] = 4;
			sendQueueGuest.emplace_back(INPUT_EMOTION);
			break;
		case '5':
			m_emotionNumber[m_idx] = 5;
			sendQueueGuest.emplace_back(INPUT_EMOTION);
			break;
		case '6':
			m_emotionNumber[m_idx] = 6;
			sendQueueGuest.emplace_back(INPUT_EMOTION);

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