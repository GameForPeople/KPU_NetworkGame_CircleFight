#pragma once

#include "Scene.h"
#include "InGameSceneUI.h"

#include <vector>

#define PLAT_MAX_NUMBER 1015
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

	CImage*						m_platImg[2];

	InGameSceneUI*				m_inGameUI;
		
	int							m_numPlat;

	int							m_emotionTimer{ 0 };
	int							m_emotionNumber{ 0 };

public:
	InGameScene();
	InGameScene(HWND);
	InGameScene(HWND, MapName, CharacterName);

	virtual ~InGameScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double time) ;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) ;
	virtual void Destory();

public:
	void LoadPlat();
	void ComputePawn();
	void EmotionUIProc();

public:
	//for network
	//virtual void NetworkFunction();

public:
	//for debug
	void ShowPawnState() const;	
};

