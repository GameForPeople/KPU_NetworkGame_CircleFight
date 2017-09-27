#include <iostream>
#include <fstream>

#include "ReplayFunction.h"

using namespace std;

/*
long long SaveCont::Encoding() {
	long long returnValue = 0;
	returnValue += posX * 100000000000 + posY * 100000000 + mouseX * 10000 + mouseY * 10 + dir;
	
	//1100 900 1200 500 1
	//110090012005001
	//              0
	//           0000
	//       00000000
	//    00000000000


	if (isShot)
	returnValue += 1;

	return returnValue;
}
*/

/*
SaveCont::SaveCont(long long saveValue) {
	posX = saveValue / 100000000000;
	posY = ( saveValue / 100000000 ) % 1000;
	mouseX = (saveValue / 10000) % 10000;
	mouseY = (saveValue / 10) % 1000;

	dir = (saveValue & 10);
}
*/

void CRep::SaveFrame(const bool up, const bool down, const bool left, const bool right, const int mouseX, const int mouseY, const int dir, const int hp) {
	replayData.emplace_back(up, down, left, right, mouseX, mouseY, dir, hp);
}

void CRep::InitData() {	
		m_frameCount = 0;
		m_nowFrame = 0;

		m_startFrame = 0;
		m_endFrame = 0;

		m_close = true;

		m_dataSize = 0;

		replayData.erase(replayData.begin(), replayData.end());
}

void CRep::SaveReplayData() {
	ofstream outFile("PlayerData.replay", ios::out);

	outFile << replayData.size() << endl;

	for (auto D : replayData) {
		outFile << " " << D.getInputUp()
			<< " " << D.getInputDown()
			<< " " << D.getInputLeft()
			<< " " << D.getInputRight()
			<< " " << D.getMouseX()
			<< " " << D.getMouseY()
			<< " " << D.getDir()
			<< " " << D.getBossHP()
			<< endl;
	}
	//어떻게 컨테이너를 초기화할 것인가?
	//replayData.vector<SaveCont>::~vector();	// 소멸 후 생성..?
	replayData.erase(replayData.begin(), replayData.end());
	outFile.close();
}

void CRep::LoadReplayData() {
	ifstream inFile("PlayerData.replay", ios::in);

	int sizeBuffer;
	int mx, my, dir, hp;
	bool up, down, left, right;

	inFile >> sizeBuffer;
	setFrameCount(sizeBuffer);

	for (int i = 0; i < sizeBuffer; i++) {
		inFile >> up >> down >> left >> right >> mx >> my >> dir >> hp;

		replayData.emplace_back(up, down, left, right, mx, my, dir, hp);
	}
	inFile.close();
}