#pragma once
#include "Scene.h"
class LoginScene :
	public Scene
{
public:
	LoginScene();
	LoginScene(HWND);
	~LoginScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);

	//virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

public:
	void LoadCImage();

private:

	CImage m_backImg;
	CImage m_logoImg;
	CImage m_pawnImg;
	CImage m_uiImg;

	bool m_isDrawLogo{ true };
	bool m_isDrawPawn{ false };
	bool m_isDrawUI{ false };

	int m_logoTimer{ 0 };
	int m_pawnTimer{ 0 };
	int m_uiTimer{ 0 };
	int m_uiOnoff{ 0 };

};

