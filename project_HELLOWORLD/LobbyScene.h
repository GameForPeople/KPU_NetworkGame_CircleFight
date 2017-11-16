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
	
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


public:
	void LoadCImage();


private:
	CImage m_backImg;
	TCHAR	m_chatBuf[5][100] = { NULL };
	TCHAR	m_chat[100] = { NULL };
	size_t	m_Len = 0;
};

