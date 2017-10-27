#pragma once
#include "Scene.h"

#include <vector>

#define PLAT_MAX_NUMBER 150
#define PLAT_WIDTH 100
#define PLAT_HEIGHT 50
#define PLAT_LOW_HEIGHT 600

class InGameScene :
	public Scene
{
public:
	InGameScene();
	InGameScene(HWND);

	virtual ~InGameScene();

	virtual void Draw(HDC hdc);
	virtual void Timer(double count) ;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) ;
	virtual void Destory();

	virtual void LoadPlat();
	virtual void ComputePawn();

	//for network
	//virtual void NetworkFunction();

	//for debug
	void ShowPawnState();	
private:
	Pawn*						m_characterArr;
	Map*						m_map;

	//std::vector<BaseObject>*	m_platArr;
	BaseObject*					m_platArr;

	CImage*						m_platImg;

	int							m_numPlat;

};

