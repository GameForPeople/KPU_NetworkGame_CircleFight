#pragma once

#include "stdafx.h"

#include "Pawn.h"
#include "Map.h"
#include "StaticActor.h"
#include "BaseUI.h"
#include "Network.h"

class Scene
{
public:
	Scene();
	Scene(HWND);
	virtual ~Scene();

	virtual void Draw(HDC hdc) = 0;
	virtual void Timer(const double count) = 0;
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	
	virtual void Destory();

protected:
	HWND			m_hWnd;
	Network*		m_network;
public:
	SceneName		m_nextScene;
	bool			m_isDestory{ false };
	bool			GetIsDestory() { return m_isDestory; }
	Network&		GetNetwork() { return *m_network; }
public:
	virtual MapName		GetMapName() { return MapName::Forest;  };	//Only Use Room -> InGame
	virtual CharacterName GetCharacterName() { return CharacterName::Archer;  }; //Only Use Room -> InGame
};

