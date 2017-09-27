//#pragma once
#ifndef __REPLAYFUNCTION_H_
#define __REPLAYFUNCTION_H_

#include <iostream>
#include <vector>

class SaveCont {
	int mouseX;
	int mouseY;
	//bool isShot;
	int dir;

	int bossHP;

	bool inputUp;
	bool inputDown;
	bool inputLeft;
	bool inputRight;

public:
	SaveCont(bool _inputUp, bool _inputDown, bool _inputLeft, bool _inputRight, int mX, int mY, int dir, int hp) : inputUp(_inputUp), inputDown(_inputDown), inputLeft(_inputLeft), inputRight(_inputRight), mouseX(mX), mouseY(mY), dir(dir), bossHP(hp) {};

	SaveCont() = default;
	~SaveCont() = default;
	//int getPosX() const { return posX; }
	//int getPosY() const { return posY; }
	int getMouseX() const { return mouseX; }
	int getMouseY() const { return mouseY; }
	int getDir() const { return dir; }
	int getBossHP() const { return bossHP; }

	bool getInputUp() const { return inputUp; }
	bool getInputDown() const { return inputDown; }
	bool getInputLeft() const { return inputLeft; }
	bool getInputRight() const { return inputRight; }

	//long long Encoding();	
};

class CRep {

	int m_frameCount = 0;
	int m_nowFrame = 0;

	int m_startFrame = 0;
	int m_endFrame = 0;

	bool m_close = true;

	int m_dataSize = 0; // ���÷��� �ε� ��! cont�� ũ�⸦ �޾Ƽ� ����
	
						//bool isSave = 0;	//�÷��̾ ������ ���� ��� 1�� ����! ���� ����� 0���� �ٲ��ָ鼭 1���ϋ����� �� ���� ��� �ؽ�Ʈ�� ���� �ʿ�!!

	std::vector<SaveCont> replayData;

public:
	CRep() = default;
	~CRep() = default;

	int getFrameCount() const { return m_frameCount; }
	int getNowFrame() const { return m_nowFrame;  }
	bool getClose() const { return m_close; }
	SaveCont& getReplayData() { return replayData[m_nowFrame]; }

	void setFrameCount(const int size) { m_frameCount = size; }
	void setNowFrame(const int num) { m_nowFrame = num;  }
	
	void InitData();
	void PlusNowframe() { m_nowFrame++; }
	//bool getIsSave() const { return isSave; }

	//void StartSave() { isSave = 1; }
	void EndSave() { SaveReplayData(); m_close = false;  }

	void SaveFrame(const bool up, const bool down, const bool left, const bool right, const int mouseX, const int mouseY, const int dir, const int hp);

	void SaveReplayData();
	void LoadReplayData();
};
#endif // !__REPLAYFUNCTION_H_
