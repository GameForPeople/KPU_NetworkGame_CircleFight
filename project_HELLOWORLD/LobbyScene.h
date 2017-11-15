#pragma once
#include "Scene.h"
class LobbyScene :
	public Scene
{
public:
	LobbyScene();
	LobbyScene(HWND);
	~LobbyScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);
	
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


public:
	void LoadCImage();


private:
	CImage m_backImg;

};

