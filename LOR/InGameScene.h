#pragma once

#include "Scene.h"
#include "InGameSceneUI.h"
#include "ItemTimer.h"
#include "RandClass.h"

class InGameScene :
	public Scene
{
private:
	//std::vector<BaseObject>	m_platArr;
	BaseObject*					m_platArr;
	BaseObject*					m_itemArr;

	Pawn						m_characterArr[MAX_PLAYER];
	Map*						m_map;

	CImage*						m_platImg[2];
	CImage*						m_itemImg;

	int							m_emotionNumber[MAX_PLAYER];
	int							m_emotionTimer[MAX_PLAYER];

	InGameSceneUI*				m_inGameUI;

	bool						m_isEatingItam;
	list<ItemTimer>				timerList;
	int							m_stackSheild[2];

	RandClass					m_rand;

public:
	InGameScene();
	InGameScene(HWND, Network *network);

	virtual ~InGameScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double time) ;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) ;
	virtual void Destory();

public:
	void LoadPlat();
	void ComputePawn(int idx);
	void EmotionUIProc();

public:
	bool BoxBoxCol(float aMinX, float aMinY, float aMaxX, float aMaxY, float bMinX, float bMinY, float bMaxX, float bMaxY);
	void UseItem(int itemNum, int user);

	void CollideItem(int idx);

	void UpdateItemList(double time);

	void SheildCountUp(int team) { m_stackSheild[team]++; }
	void SheildCountDown(int team) 	{ m_stackSheild[team]--; }

public:
	//for debug
	void ShowPawnState();	
};

