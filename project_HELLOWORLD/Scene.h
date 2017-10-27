#pragma once

#include "stdafx.h"

#include "Pawn.h"
#include "Map.h"
#include "StaticActor.h"
#include "BaseUI.h"

class Scene
{
public:
	Scene();
	Scene(HWND);
	virtual ~Scene();

	virtual void Draw(HDC hdc) = 0;
	virtual void Timer(double count) = 0;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) = 0;
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam) = 0;
	virtual void Destory() = 0;

protected:
	HWND			m_hWnd;
};

