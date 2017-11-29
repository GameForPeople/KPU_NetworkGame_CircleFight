#pragma once

#include "Scene.h"
#include "InGameSceneUI.h"
#include "NetworkFunc.h"

#include <vector>

#define PLAT_MAX_NUMBER 200
#define PLAT_WIDTH 100
#define PLAT_HEIGHT 50
#define PLAT_LOW_HEIGHT 600

#define PLAT_SHOWN_CNT 16


class InGameSceneGuest :
	public Scene
{
private:

	//std::vector<BaseObject>	m_platArr;
	BaseObject*					m_platArr;
	Pawn						m_characterArr[MAX_PLAYER];
	Map*						m_map;

	CImage*						m_platImg[2];

	InGameSceneUI*				m_inGameUI;

	int	m_emotionNumber[MAX_PLAYER];
	int m_emotionTimer[MAX_PLAYER];

	int							m_numPlat;

public:
	InGameSceneGuest();
	InGameSceneGuest(HWND);
	InGameSceneGuest(HWND, MapName, CharacterName*);

	virtual ~InGameSceneGuest();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double time);
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual void Destory();

public:
	void LoadPlat();

public:
	//for network
	//virtual void NetworkFunction();

public:
	//for debug
	void ShowPawnState();
};

