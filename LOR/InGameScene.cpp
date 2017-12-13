#include "InGameScene.h"
#include <fstream>

#define TO_DEBUG_ARCHER
//#define TO_DEBUG_ZOMBIE
//#define TO_DEBUG_KNIGHT
//#define TO_DEBUG_WICHER

//#define TO_DEBUG_MAP_SEA
#define TO_DEBUG_MAP_FOREST

InGameScene::InGameScene(HWND hwnd, Network* network) : Scene(hwnd)
{
	m_network = network;
	SuspendThread(m_network->m_networkThread);
	//BGM재생
	m_network->m_nowBgmNumber = 1;
	m_network->m_system->playSound(FMOD_CHANNEL_REUSE, m_network->m_sound[1], false, &(m_network->m_channel[0]));

	for (int i = 1; i<MAX_PLAYER; ++i) sendQueue[i].clear();
	memset(emotionNum, 0, sizeof(emotionNum));
	memset(emotionTime, 0, sizeof(emotionTime));
	memset(&basicInfo, 0, sizeof(basicInfo));

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (roomInfo.m_charInfo[i] == CharacterName::Archer)
			m_characterArr[i] = Pawn(CharacterName::Archer);
		else if (roomInfo.m_charInfo[i] == CharacterName::Zombie)
			m_characterArr[i] = Pawn(CharacterName::Zombie);
		else if (roomInfo.m_charInfo[i] == CharacterName::Knight)
			m_characterArr[i] = Pawn(CharacterName::Knight);
		else if (roomInfo.m_charInfo[i] == CharacterName::Wicher)
			m_characterArr[i] = Pawn(CharacterName::Wicher);
	}


	m_characterArr->SetState(State::Fall);

	if (roomInfo.m_mapInfo == MapName::Sea)
		m_map = new Map(0, 0, "Resource/Image/Background/Background.png");
	else if (roomInfo.m_mapInfo == MapName::Forest)
		m_map = new Map(0, 0, "Resource/Image/Background/Background_2.png");


	m_platImg[0].Load("Resource/Image/Plat/Plat_2.png");
	m_platImg[1].Load("Resource/Image/Plat/Plat_1.png");

	m_itemImg.Load("Resource/Image/UI/item_Random_2.png");

	m_winLoseImg[0].Load("Resource/Image/UI/Win.png");
	m_winLoseImg[1].Load("Resource/Image/UI/Lose.png");

	int numItem = (PLAT_MAX_NUMBER - ITEM_FIRST_IMPACT) / ITEM_INTERVAL + 1;

	m_platArr = new BaseObject[PLAT_MAX_NUMBER];
	m_itemArr = new BaseObject[numItem];

	m_stackSheild[0] = 0;
	m_stackSheild[1] = 0;

	LoadPlat();

	charArr = m_characterArr;

	m_resultUICount = 0;
	m_isGameEnd = false;
	m_network->m_gameResult = 0;
	m_network->m_gameResultBuffer = 0;
}

InGameScene::InGameScene()
{
}

InGameScene::~InGameScene()
{
	//ResumeThread(m_network->m_networkThread);
	if (m_map) delete m_map;
	if (m_platArr) delete[] m_platArr;
	if (m_itemArr) delete[] m_itemArr;
}



void InGameScene::Draw(HDC hdc) {
	m_map->Draw(hdc);

	float firstPos = basicInfo.m_firstPlat[0].xPos;
	for (int i = basicInfo.m_firstPlat[0].idx, j = 0; j < PLAT_SHOWN_CNT; ++i, ++j) {
		if (m_platArr[i].GetPos().y != PLAT_LOW_HEIGHT)
			m_platImg[0].TransparentBlt(hdc, firstPos + PLAT_WIDTH * j, m_platArr[i].GetPos().y, PLAT_WIDTH, PLAT_HEIGHT, RGB(255, 255, 255));
		else
			m_platImg[1].TransparentBlt(hdc, firstPos + PLAT_WIDTH * j, m_platArr[i].GetPos().y, PLAT_WIDTH, m_platImg[1].GetHeight(), RGB(255, 255, 255));
	}

	m_itemImg.TransparentBlt(hdc, basicInfo.m_firstItem[0].xPos, m_itemArr[basicInfo.m_firstItem[0].idx].GetPos().y, ITEM_SIZE, ITEM_SIZE, RGB(255, 255, 255));

	for (int i = 1; i < MAX_PLAYER; ++i)
	{
		m_characterArr[i].NetworkDrawCharacter(hdc, basicInfo.m_totalDis[0], basicInfo.m_totalDis[i], basicInfo.m_yPos[i],
			basicInfo.m_imgCnt[i], basicInfo.m_state[i]);
	}
	m_characterArr[0].NetworkDrawCharacter(hdc, 0, 0, basicInfo.m_yPos[0],
		basicInfo.m_imgCnt[0], basicInfo.m_state[0]);

	m_inGameUI.DrawComboUI(hdc, m_characterArr[0].GetCombo());
	m_inGameUI.DrawInventoryUI(hdc, basicInfo.m_itemInfo[0][0], basicInfo.m_itemInfo[0][1]);

	for (int i = 1; i < MAX_PLAYER; ++i)
	{
		m_inGameUI.DrawBarUI(hdc, i, m_characterArr[i].GetTotalDistance() / 110);
		m_inGameUI.DrawPlayerMark(hdc, i, basicInfo.m_yPos[i], basicInfo.m_totalDis[0], basicInfo.m_totalDis[i]);
		if (emotionNum[i])
			m_inGameUI.DrawEmotionUI(hdc, emotionNum[i], basicInfo.m_totalDis[0], basicInfo.m_totalDis[i], basicInfo.m_yPos[i]);
		else
			m_inGameUI.DrawHeadUpUI(hdc, m_characterArr[i].GetPos().y, basicInfo.m_totalDis[0], basicInfo.m_totalDis[i], i);
	}
	m_inGameUI.DrawBarUI(hdc, 0, m_characterArr[0].GetTotalDistance() / 110);
	m_inGameUI.DrawPlayerMark(hdc, 0, basicInfo.m_yPos[0]);
	if (emotionNum[0])
		m_inGameUI.DrawEmotionUI(hdc, emotionNum[0], 0, 0, basicInfo.m_yPos[0]);
	else
		m_inGameUI.DrawHeadUpUI(hdc, m_characterArr[0].GetPos().y, 0, 0, 0);

	m_inGameUI.DrawRankUI(hdc);

	if (m_gameResult == 1) m_winLoseImg[0].TransparentBlt(hdc, 200, 200, 880, 320, RGB(255, 0, 255));
	if (m_gameResult == 2) m_winLoseImg[1].TransparentBlt(hdc, 200, 200, 880, 320, RGB(0, 255, 0));
}

void InGameScene::Timer(const double time) {
	
	if (m_resultUICount > 0)
	{
		m_resultUICount++;
		if (m_resultUICount > 300)
		{
			m_nextScene = SceneName::Room;
			m_isDestory = true;
		}
	}
	
	UpdateItemList(time);
	
	while (!itemQueue.empty())
	{
		UseItem(itemQueue.front().itemNum, itemQueue.front().userIdx);
		itemQueue.pop_front();
	}

	m_map->Update(m_characterArr[0].GetSpeed(), time);

	for (int i = 0; i < MAX_PLAYER; ++i)
		m_characterArr[i].Update(m_characterArr[i].GetState(), time);

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		float spd = m_characterArr[i].GetSpeed();

		basicInfo.m_firstPlat[i].xPos -= spd;
		basicInfo.m_firstItem[i].xPos -= spd;

		if (basicInfo.m_firstPlat[i].xPos < -PLAT_WIDTH)
		{
			basicInfo.m_firstPlat[i].xPos += PLAT_WIDTH;
			basicInfo.m_firstPlat[i].idx++;
		}
		if (basicInfo.m_firstItem[i].xPos < -PLAT_WIDTH)
		{
			basicInfo.m_firstItem[i].xPos += ITEM_INTERVAL * PLAT_WIDTH;
			basicInfo.m_firstItem[i].idx++;
		}
	}

	EmotionUIProc();

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		ComputePawn(i);
		CollideItem(i);
		FinishChecker(i);
		basicInfo.m_yPos[i] = m_characterArr[i].GetYPos();
		basicInfo.m_totalDis[i] = m_characterArr[i].GetTotalDistance();
		basicInfo.m_speed[i] = m_characterArr[i].GetSpeed();
		basicInfo.m_state[i] = m_characterArr[i].GetState();
		basicInfo.m_imgCnt[i] = m_characterArr[i].GetUnit().GetImageCount();
		basicInfo.m_combo[i] = m_characterArr[i].GetCombo();
	}

	for (int i = 1; i < MAX_PLAYER; ++i)
	{
		sendQueue[i].emplace_back(UPDATE_FRAME, 0);
	}
	//ShowPawnState();	//Debug
}

bool InGameScene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//if (m_characterArr->InsertKey(wParam)) return true;
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (m_characterArr[0].InsertKey(wParam))return true;

		switch (wParam) {
		case VK_SPACE:

			break;
		case 'G':
			std::cout << charArr[0].GetTotalDistance() << std::endl;
			break;
		case '1':
			emotionNum[0] = 1;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { 	sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case '2':
			emotionNum[0] = 2;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case '3':
			emotionNum[0] = 3;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case '4':
			emotionNum[0] = 4;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case '5':
			emotionNum[0] = 5;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case '6':
			emotionNum[0] = 6;
			emotionTime[0] = 1;
			for (int i = 1; i < MAX_PLAYER; ++i) { sendQueue[i].emplace_back(CHANGE_EMOTION, 0); }
			break;
		case 'Q':					//1번 아이템 사용
		case 'q':
			if (m_characterArr[0].GetFaint() > 0 && basicInfo.m_itemInfo[0][0] != WING) { break; }
			UseItem(basicInfo.m_itemInfo[0][0], 0);
			basicInfo.m_itemInfo[0][0] = -1;
			break;
		case 'W':					//2번 아이템 사용
		case 'w':
			if (m_characterArr[0].GetFaint() > 0 && basicInfo.m_itemInfo[0][1] != WING) { break; }
			UseItem(basicInfo.m_itemInfo[0][1], 0);
			basicInfo.m_itemInfo[0][1] = -1;
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

bool InGameScene::MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	return true;
}

void InGameScene::Destory() {

}


void InGameScene::LoadPlat() {
	//ifstream inFile("Resource/Data/platData.txt", ios::in);
	ifstream inFile("Resource/Data/NewPlatData.txt", ios::in);
	//to debug
	//ifstream inFile("platData.txt", ios::in);	

	int posX = 1;
	int posY = 0;

	for (int i = 0, j = 0; i < PLAT_MAX_NUMBER; i++) {

		inFile >> posY;

		m_platArr[i].SetPos(3000, (posY * (-100)) + PLAT_LOW_HEIGHT);
		if (i % ITEM_INTERVAL == ITEM_FIRST_IMPACT)
		{
			m_itemArr[j].SetPos(3000, (posY * (-100)) + PLAT_LOW_HEIGHT - PLAT_ITEM_HEIGHT);
			j++;
		}
		//to debug
		//std::cout << i << "번째  " << posY << "   " << m_platArr[i].GetPos().x << "   " << m_platArr[i].GetPos().y << std::endl;
	}
	inFile.close();

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		basicInfo.m_firstPlat[i].idx = 0;
		basicInfo.m_firstPlat[i].xPos = 0;

		basicInfo.m_firstItem[i].idx = 0;
		basicInfo.m_firstItem[i].xPos = ITEM_FIRST_IMPACT * PLAT_WIDTH;

		basicInfo.m_itemInfo[i][0] = basicInfo.m_itemInfo[i][1] = -1;
	}



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

void InGameScene::ComputePawn(int idx) {
	//std::cout << m_characterArr->GetTotalDistance() / 100 + 2 << std::endl;
#pragma region [캐릭터의 Y값을 계산합니다.]

	//	int leftPlat = (int)(m_characterArr[idx].GetTotalDistance() / PLAT_WIDTH + 2);
	//int rightPlat = (int)(m_characterArr[idx].GetTotalDistance() / PLAT_WIDTH + 3);
	int rightPlat = basicInfo.m_firstPlat[idx].idx + 3;

	float pawnPosY = m_characterArr[idx].GetPos().y + 185;

	if (m_characterArr[idx].GetState() == State::Fall
		|| m_characterArr[idx].GetState() == State::JumpEnd
		|| m_characterArr[idx].GetState() == State::DoubleJumpEnd
		|| m_characterArr[idx].GetState() == State::TripleJumpEnd
		) {
		//if (m_platArr[leftPlat].GetPos().y - 5<= pawnPosY
		//	&& m_platArr[leftPlat].GetPos().y + 5 > pawnPosY) {
		//
		//	m_characterArr->SetPos(200, m_platArr[leftPlat].GetPos().y - 185);
		//	m_characterArr->SetState(State::Run);
		//	m_characterArr->GetUnit().SetImageCount(0);
		//}
		//else 

		if (m_platArr[rightPlat].GetPos().y - 10 <= pawnPosY
			&& m_platArr[rightPlat].GetPos().y + 30 > pawnPosY) {

			m_characterArr[idx].SetPos(200, m_platArr[rightPlat].GetPos().y - 185);
			m_characterArr[idx].SetState(State::Run);
			m_characterArr[idx].GetUnit().SetImageCount(0);
			m_characterArr[idx].ResetFallSpeed();
		}
		else if (pawnPosY > SCREEN_HEIGHT + 50) {
			m_characterArr[idx].ResetCombo();
			m_characterArr[idx].SetPos(200, 0);
			m_characterArr[idx].SetState(State::JumpEnd);
		}
	}
	else if (m_characterArr[idx].GetState() == State::Run) {

		bool isValue = false;
		//if (m_platArr[leftPlat].GetPos().y - 5 <= pawnPosY
		//	&& m_platArr[leftPlat].GetPos().y + 5 > pawnPosY) {
		//}
		//else 
		if (m_platArr[rightPlat].GetPos().y - 10 <= pawnPosY
			&& m_platArr[rightPlat].GetPos().y + 30 > pawnPosY) {
		}
		else {
			m_characterArr[idx].SetState(State::Fall);
		}
	}
	//else m_characterArr->SetState(State::Fall);
	//to debug
#pragma endregion

}

void InGameScene::ShowPawnState() {

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

void InGameScene::EmotionUIProc() {
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (emotionNum[i]) {
			emotionTime[i]++;

			if (emotionTime[i] >= 90) {
				emotionTime[i] = 0;
				emotionNum[i] = 0;
				for (int j = 1; j < MAX_PLAYER; ++j) 
				{ 
					if (j == m_idx) continue;
					sendQueue[j].emplace_back(CHANGE_EMOTION, i); }
			}
		}
	}
}


bool InGameScene::BoxBoxCol(float aMinX, float aMinY, float aMaxX, float aMaxY, float bMinX, float bMinY, float bMaxX, float bMaxY)
{
	if (aMinX > bMaxX)
		return false;
	if (aMaxX < bMinX)
		return false;

	if (aMinY > bMaxY)
		return false;
	if (aMaxY < bMinY)
		return false;

	return true;
}

void InGameScene::UseItem(int itemNum, int user)
{
	if (itemNum == -1) return;

	static int b, e;
	static int itemIdx;

	if (itemNum == LIGHTNING)		// 번개
	{
		//send()
		ItemTimer elem(LIGHTNING_DELAY, LIGHTNING, user);
		timerList.push_back(elem);
		if (user < 2) { b = 2;	e = 4; }
		else { b = 0;	e = 2; }
		itemIdx = NOTIFY_ITEM_THUNDER;
	}
	else if (itemNum == BED)	// 숙면
	{
		ItemTimer elem(BED_DELAY, BED, user);
		timerList.push_back(elem);
		if (user < 2) { b = 2;	e = 4; }
		else { b = 0;	e = 2; }
		itemIdx = NOTIFY_ITEM_BED;
	}
	else if (itemNum == WING)	// 날개
	{
		ItemTimer elem(WING_DELAY, WING, user);
		timerList.push_back(elem);
		if (user < 2) { b = 0;	e = 2; }
		else { b = 2;	e = 4; }
		itemIdx = NOTIFY_ITEM_WING;
	}
	else if (itemNum == SHEILD)	// 방패
	{
		ItemTimer elem(SHEILD_DELAY, SHEILD, user);
		timerList.push_back(elem);
		if (user < 2) { b = 0;	e = 2; }
		else { b = 2;	e = 4; }
		itemIdx = NOTIFY_ITEM_SHIELD;
	}

	for (; b < e; ++b)
	{
		if (b == 0) { 
			/*소리 재생*/ 
			if(itemNum == LIGHTNING)
				PlaySound("Resource\\Sound\\ets.wav", NULL, SND_ASYNC);
			else if (itemNum == BED)
				PlaySound("Resource\\Sound\\sleeping.wav", NULL, SND_ASYNC);
			else if (itemNum == SHEILD)
			{
				PlaySound("Resource\\Sound\\shield.wav", NULL, SND_ASYNC);
				m_inGameUI.SetUI(0, SHEILD);
			}
			else if (itemNum == WING)
			{
				PlaySound("Resource\\Sound\\angle.wav", NULL, SND_ASYNC);
				m_inGameUI.SetUI(0, WING);
			}
		}
		else 
		{ 
			sendQueue[b].emplace_back(itemIdx, user); 
			if(itemNum == SHEILD) m_inGameUI.SetUI(b, SHEILD);
			if (itemNum == WING) m_inGameUI.SetUI(b, WING);
		}
	}

	if (itemNum != WING && itemNum != SHEILD) return;

	for (int i = 1; i < MAX_PLAYER; ++i)
	{
		if (itemNum == WING) sendQueue[i].emplace_back(SET_UI_WING, user);
		if (itemNum == SHEILD) sendQueue[i].emplace_back(SET_UI_SHEILD, user);
	}
}

void InGameScene::CollideItem(int idx)
{
	static float itemHalfSize = ITEM_SIZE / 2;
	Pos2d charPos = m_characterArr[idx].GetPos();

	float charSize = 200;

	float aMinX = charPos.x;
	float aMinY = charPos.y;
	float aMaxX = charPos.x + charSize;
	float aMaxY = charPos.y + charSize;

	float itemX = basicInfo.m_firstItem[idx].xPos;
	int itemIdx = basicInfo.m_firstItem[idx].idx;

	float bMinX = itemX - itemHalfSize;
	float bMinY = m_itemArr[itemIdx].GetPos().y;
	float bMaxX = itemX + itemHalfSize;
	float bMaxY = m_itemArr[itemIdx].GetPos().y + ITEM_SIZE;

	if (BoxBoxCol(aMinX, aMinY, aMaxX, aMaxY, bMinX, bMinY, bMaxX, bMaxY))
	{
		if (m_isEatingItam[idx]) { return; }	//한번만 먹으라
		int emptyIdx = (basicInfo.m_itemInfo[idx][0] == -1) ? 0 : (basicInfo.m_itemInfo[idx][1] == -1) ? 1 : -1;
		if (emptyIdx != -1) basicInfo.m_itemInfo[idx][emptyIdx] = m_rand.getRandomNumber(1, 4);
		m_isEatingItam[idx] = true;
		return;							//하나 이상 충돌할일이 없음
	}

	m_isEatingItam[idx] = false;
}

void InGameScene::UpdateItemList(double time)
{
	if (timerList.empty()) return;

	int b, e;
	float duration;
	bool wingBoost = false;
	list<ItemTimer>::iterator iter;

	for (iter = timerList.begin(); iter != timerList.end();)
	{
		ItemTimer* elem;
		int iterIdx = iter->getUserIdx();
		int zbRand[MAX_PLAYER]{ -1, -1, -1, -1 };
		switch (iter->update(time))
		{
		case LIGHTNING:																// 번개 - 선딜 1.5초, 기절 2초
			if (iterIdx < 2 && m_stackSheild[0] > 0) { iter = timerList.erase(iter);  break; }
			else if (m_stackSheild[1] > 0) { iter = timerList.erase(iter); break; }

			if (m_characterArr[iterIdx].GetCharType() == CharacterName::Knight) { duration = LIGHTNING_DURATION_B; }
			else { duration = LIGHTNING_DURATION; }

			if (iterIdx < 2) { b = 2;	e = 4; }
			else { b = 0;	e = 2; }

			for (; b < e; ++b)
			{
				if (m_characterArr[b].GetCharType() == CharacterName::Zombie && m_rand.getRandomNumber(0, 1) == 0) { continue; }
				m_characterArr[b].FaintCountUp(true);
				m_inGameUI.SetUI(b, LIGHTNING);

				timerList.push_back(ItemTimer(duration, TIMEOUT_FAINT, b));
				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					sendQueue[i].emplace_back(SET_UI_THUNDER, b);
				}
			}
			iter = timerList.erase(iter);
			break;
		case BED:								// 침대 - 선딜 1초, 기절 3초
			if (iterIdx < 2 && m_stackSheild[0] > 0) { iter = timerList.erase(iter);  break; }
			else if (m_stackSheild[1] > 0) { iter = timerList.erase(iter); break; }

			if (m_characterArr[iterIdx].GetCharType() == CharacterName::Knight) { duration = BED_DURATION_B; }
			else { duration = BED_DURATION; }

			if (iterIdx < 2) { b = 2;	e = 4; }
			else { b = 0;	e = 2; }

			for (; b < e; ++b)
			{
				if (m_characterArr[b].GetCharType() == CharacterName::Zombie && m_rand.getRandomNumber(0, 1) == 0) { continue; }
				m_characterArr[b].FaintCountUp(false);
				m_inGameUI.SetUI(b, BED);
				timerList.push_back(ItemTimer(duration, TIMEOUT_SLEEP, b));
				for (int i = 1; i < MAX_PLAYER; ++i)
				{
					sendQueue[i].emplace_back(SET_UI_BED, b);
				}
			}
			iter = timerList.erase(iter);
			break;
		case SHEILD:						// 방패 - 선딜 0초, 보호 3초
			if (m_characterArr[iterIdx].GetCharType() == CharacterName::Wicher) { duration = SHEILD_DURATION_B; }
			else { duration = SHEILD_DURATION; }

			if (iterIdx < 2) { m_stackSheild[0]++; }
			else { m_stackSheild[1]++; }

			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				sendQueue[i].emplace_back(SET_UI_SHEILD, iterIdx);
			}

			timerList.push_back(ItemTimer(duration, TIMEOUT_PROTECT, iterIdx));
			iter = timerList.erase(iter);
			break;
		case WING:								// 날개 - 선딜 0초, 이속1.2 3초, 상태이상 해제
			if (m_characterArr[iterIdx].GetCharType() == CharacterName::Wicher) { wingBoost = true; duration = WING_DURATION_B; }
			else { duration = WING_DURATION; }

			if (iterIdx < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }

			for (; b < e; ++b)
			{
				m_characterArr[b].FaintReset();
				m_characterArr[b].SpeedUpCountUp(wingBoost);
			}

			for (int i = 1; i < MAX_PLAYER; ++i)
			{
				sendQueue[i].emplace_back(SET_UI_WING, iterIdx);
			}

			timerList.push_back(ItemTimer(duration, TIMEOUT_SPEEDUP, iterIdx));
			iter = timerList.erase(iter);
			break;
		case TIMEOUT_FAINT:
			m_characterArr[iterIdx].FaintCountDown();
			m_inGameUI.ResetUI(iterIdx, LIGHTNING);
			for (int i = 1; i < MAX_PLAYER; ++i) sendQueue[i].emplace_back(RESET_UI_THUNDER, iterIdx);
			iter = timerList.erase(iter);
			break;
		case TIMEOUT_SLEEP:
			m_characterArr[iterIdx].FaintCountDown();
			m_inGameUI.ResetUI(iterIdx, BED);
			for (int i = 1; i < MAX_PLAYER; ++i) sendQueue[i].emplace_back(RESET_UI_BED, iterIdx);
			iter = timerList.erase(iter);
			break;
		case TIMEOUT_PROTECT:
			if (iterIdx < 2) { b = 0;	e = 2;  m_stackSheild[0]--; }
			else { b = 2;	e = 4; m_stackSheild[1]--; }
			for (; b < e; ++b) m_inGameUI.ResetUI(b, SHEILD); 
			for (int i = 1; i < MAX_PLAYER; ++i) sendQueue[i].emplace_back(RESET_UI_SHEILD, iterIdx);
			iter = timerList.erase(iter);
			break;
		case TIMEOUT_SPEEDUP:
			if (iterIdx < 2) { b = 0;	e = 2; }
			else { b = 2;	e = 4; }

			for (; b < e; ++b)
			{
				m_inGameUI.ResetUI(b, WING);
				m_characterArr[b].SpeedUpCountDown();
			}
			for (int i = 1; i < MAX_PLAYER; ++i) sendQueue[i].emplace_back(RESET_UI_WING, iterIdx);
			iter = timerList.erase(iter);
			break;
		default:
			iter++;
			break;
		}
	}
}

void InGameScene::FinishChecker(int idx)
{
	if (m_isGameEnd == false) {
		if (m_characterArr[idx].GetTotalDistance() < 100000) return;
		if (m_resultUICount == 0) m_resultUICount++;
		if (idx < 2)
		{
			m_gameResult = m_network->m_gameResult = 1;
			sendQueue[1].emplace_back(NOTIFY_WIN);
			sendQueue[2].emplace_back(NOTIFY_LOSE);
			sendQueue[3].emplace_back(NOTIFY_LOSE);
		}
		else
		{
			m_gameResult = m_network->m_gameResult = 2;
			sendQueue[1].emplace_back(NOTIFY_LOSE);
			sendQueue[2].emplace_back(NOTIFY_WIN);
			sendQueue[3].emplace_back(NOTIFY_WIN);
		}

		ResumeThread(m_network->m_networkThread); //--> 소멸자로 다시보냄!
		m_isGameEnd = true;
	}
}