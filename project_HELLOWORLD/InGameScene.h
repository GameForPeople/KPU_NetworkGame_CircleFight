#pragma once

#include "Scene.h"
#include "InGameSceneUI.h"

#include <vector>

#define PLAT_MAX_NUMBER 150
#define PLAT_WIDTH 100
#define PLAT_HEIGHT 50
#define PLAT_LOW_HEIGHT 600

class InGameScene :
	public Scene
{
private:
	Pawn*						m_characterArr;
	Map*						m_map;
						
	//std::vector<BaseObject>	m_platArr;
	BaseObject*					m_platArr;

	CImage*						m_platImg;

	InGameSceneUI*				m_inGameUI;
		
	int							m_numPlat;

public:
	InGameScene();
	InGameScene(HWND);

	virtual ~InGameScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double time) ;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) ;
	virtual void Destory();

public:
	virtual void LoadPlat();
	virtual void ComputePawn();

public:
	//for network
	//virtual void NetworkFunction();

public:
	//for debug
	void ShowPawnState() const;	
};

