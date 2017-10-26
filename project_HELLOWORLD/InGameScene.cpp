#include "InGameScene.h"
#include <fstream>

InGameScene::InGameScene(HWND hwnd) : Scene(hwnd)
{
	m_characterArr = new Pawn(CharacterName::Archer);
	m_characterArr->SetState(State::Fall);

	m_map = new Map(0, 0, "Resource/Image/Background/Background.png");
	
	m_platImg = new CImage;
	m_platImg->Load("Resource/Image/Plat/brickPlat_Green.png");

	m_numPlat = PLAT_MAX_NUMBER;
	m_platArr = new BaseObject[m_numPlat];
	LoadPlat();
}

InGameScene::InGameScene() 
{
}

InGameScene::~InGameScene()
{
}

void InGameScene::Draw(HDC hdc) {
	m_map->Draw(hdc);

	for (int i = 0; i < m_numPlat; i++) {
		m_platImg->TransparentBlt(hdc, m_platArr[i].GetPos().x, m_platArr[i].GetPos().y, PLAT_WIDTH, PLAT_HEIGHT, RGB(255,255,255));
	}

	m_characterArr->Draw(hdc, m_characterArr->GetState());
}

void InGameScene::Timer(double count){
	m_map->Update(m_characterArr->GetSpeed());
	m_characterArr->Update(m_characterArr->GetState());
	
	for( int i = 0 ; i < PLAT_MAX_NUMBER; i++)
		m_platArr[i].Update(m_characterArr->GetSpeed());

	ComputePawn();
	ShowPawnState();	//Debug
}

bool InGameScene::KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) {
	//if (m_characterArr->InsertKey(wParam)) return true;
	switch (iMessage)
	{
	case WM_KEYDOWN:
	{
		if (m_characterArr->InsertKey(wParam))return true;
		
		switch (wParam) {
		case VK_SPACE:
		
			break;
		case 'G':

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
	
	for (int i = 0; i < m_numPlat; i++) {
		
		inFile >> posY;
		
		m_platArr[i].SetPos(posX * i *  PLAT_WIDTH, (posY * (-100)) + PLAT_LOW_HEIGHT);
	
		//to debug
		std::cout << i << "번째  " << posY << "   " << m_platArr[i].GetPos().x << "   " << m_platArr[i].GetPos().y << std::endl;
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

void InGameScene::ComputePawn() {
	//std::cout << m_characterArr->GetTotalDistance() / 100 + 2 << std::endl;
#pragma region [캐릭터의 Y값을 계산합니다.]

	int leftPlat = (int)(m_characterArr->GetTotalDistance() / 100 + 2);
	int rightPlat = (int)(m_characterArr->GetTotalDistance() / 100 + 3);

	float pawnPosY = m_characterArr->GetPos().y + 185;

	if (m_characterArr->GetState() == State::Fall
		|| m_characterArr->GetState() == State::JumpEnd
		|| m_characterArr->GetState() == State::DoubleJumpEnd
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

			m_characterArr->SetPos(200, m_platArr[rightPlat].GetPos().y - 185);
			m_characterArr->SetState(State::Run);
			m_characterArr->GetUnit().SetImageCount(0);
			m_characterArr->ResetFallSpeed();
		}
		else {
			//Dead 처리
		}
	}
	else if (m_characterArr->GetState() == State::Run) {
		
		bool isValue = false;

		//if (m_platArr[leftPlat].GetPos().y - 5 <= pawnPosY
		//	&& m_platArr[leftPlat].GetPos().y + 5 > pawnPosY) {
		//}
		//else 
			if (m_platArr[rightPlat].GetPos().y - 10 <= pawnPosY
			&& m_platArr[rightPlat].GetPos().y + 30 > pawnPosY) {
		}
		else {
			m_characterArr->SetState(State::Fall);
		}
	}
		//else m_characterArr->SetState(State::Fall);
		//to debug
#pragma endregion

}

void InGameScene::ShowPawnState() {

	if (m_characterArr->GetState() == State::Run) {
		std::cout << "Run" << std::endl;
	}
	else if (m_characterArr->GetState() == State::JumpLoop) {
		std::cout << "JUMPLOOP" << std::endl;
	}
	else if (m_characterArr->GetState() == State::JumpEnd) {
		std::cout << "JUMPEnd" << std::endl;
	}
	else if (m_characterArr->GetState() == State::DoubleJumpEnd) {
		std::cout << "DoubleJUMPEnd" << std::endl;
	}

	else if (m_characterArr->GetState() == State::Fall) {
		std::cout << "Fall" << std::endl;
	}
	else {
		if (m_characterArr->GetState() == State::Boost) {
			std::cout << "Boost" << std::endl;
		}
		else if (m_characterArr->GetState() == State::Collide) {
			std::cout << "Collide" << std::endl;
		}
		else if (m_characterArr->GetState() == State::Death) {
			std::cout << "Death" << std::endl;
		}
		else if (m_characterArr->GetState() == State::DoubleJumpLoop) {
			std::cout << "DoupleJump" << std::endl;
		}
		else if (m_characterArr->GetState() == State::Sleep) {
			std::cout << "Sleep" << std::endl;
		}
		else if (m_characterArr->GetState() == State::Slow) {
			std::cout << "Slow" << std::endl;
		}
		else if (m_characterArr->GetState() == State::Stun) {
			std::cout << "Stun" << std::endl;
		}
		else if (m_characterArr->GetState() == State::TripleJump) {
			std::cout << "TripleJump" << std::endl;
		}
		else
			std::cout << "What is the Problem!!" << std::endl;
	}
}
