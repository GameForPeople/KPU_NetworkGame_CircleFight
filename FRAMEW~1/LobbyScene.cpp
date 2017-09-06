#include "stdafx.h"
#include <Windows.h>
#include <iostream>

#include "Framework.h"
#include "Unit.h"
#include "LobbyScene.h"

using namespace std;

CLobbyScene::CLobbyScene()
{

}


CLobbyScene::~CLobbyScene()
{
}

void CLobbyScene::Update(float fTimeElapsed)
{
	ProcessInput();
	m_Unit->OnUpdate(fTimeElapsed);
}

void CLobbyScene::Draw(HDC hdc)
{
	m_cimgBackground.StretchBlt(hdc, m_rcScene);

	// Unit Draw
	m_Unit->OnDraw(hdc);
}

bool CLobbyScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		}

		break;

	case WM_KEYUP:
		switch (wParam)
		{
		}

		break;
	}
	return false;
}

bool CLobbyScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_RBUTTONDOWN:
		m_pFramework->ChangeScene(CurrentScene::Title);
		return true;
	case WM_MOUSEMOVE:
		return true;
	}
	return false;
}

bool CLobbyScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_SIZE:
		break;
	case WM_DESTROY:
		break;
	}
	return false;
}

void CLobbyScene::BuildObjects(CFramework * myFramework, HWND hWnd, CurrentScene tag)
{
	CScene::BuildObjects(myFramework, hWnd, tag);
	m_cimgBackground.Load(TEXT("Resource\\Graphics\\Background\\BG_Lobby.jpg"));

	// Unit Initialize
	UnitInitialize();
}

void CLobbyScene::ReleaseObjects()
{
	if (!m_Unit) delete m_Unit;
}

inline void CLobbyScene::UnitInitialize()
{
	m_Unit = new CUnit(TEXT("Resource\\Graphics\\Sprite\\Chirno.png"), 12, 6, { 100, 500 });

	m_Unit->SetState(CAnimationSprite::SpriteState::sprite_state_idle, 6);
	m_Unit->SetState(CAnimationSprite::SpriteState::sprite_state_move, 8, 0, 1, 6, 8);
	m_Unit->SetState(CAnimationSprite::SpriteState::sprite_state_jump, 12);
	m_Unit->SetStateDelay(CAnimationSprite::SpriteState::sprite_state_idle, 5);
	m_Unit->SetStateDelay(CAnimationSprite::SpriteState::sprite_state_move, 10, 3, 8);
	m_Unit->SetStateDelay(CAnimationSprite::SpriteState::sprite_state_jump, 2);
}

bool CLobbyScene::ProcessInput()
{
	bool bProcessedByScene = false;
	if (!bProcessedByScene)
	{
		static UCHAR pKeyBuffer[256];
		DWORD dwDirection = 0;

		bool bPushShift = false;
		bool bControll = false;

		if (GetKeyboardState(pKeyBuffer))
		{
//			if (pKeyBuffer[VK_UP	] & 0xF0)	dwDirection	|= DIR_FORWARD;
//			if (pKeyBuffer[VK_DOWN	] & 0xF0)	dwDirection |= DIR_BACKWARD;
//			if (pKeyBuffer[VK_LEFT	] & 0xF0)	dwDirection |= DIR_LEFT;
//			if (pKeyBuffer[VK_RIGHT	] & 0xF0)	dwDirection |= DIR_RIGHT;
			if (pKeyBuffer[VK_LSHIFT] & 0xF0)	  bPushShift = true;
			if (pKeyBuffer[VK_LEFT	] & 0xF0)  	  dwDirection |= DIR_LEFT;	
			if (pKeyBuffer[VK_RIGHT	] & 0xF0)	  dwDirection |= DIR_RIGHT;	
			if (pKeyBuffer[VK_UP	] & 0xF0)	{ bControll = true; m_Unit->Input(CUnit::InputKey::Jump, bPushShift);		}
			if (pKeyBuffer[VK_SPACE	] & 0xF0)	{ bControll = true; m_Unit->Input(CUnit::InputKey::Jump, bPushShift);		}
			if (pKeyBuffer[VK_DOWN	] & 0xF0)	{ bControll = true; m_Unit->Input(CUnit::InputKey::KeyUp);					}

			if (!((pKeyBuffer[VK_LEFT] & 0xF0) && (pKeyBuffer[VK_RIGHT] & 0xF0)))
			{
				if(dwDirection & DIR_LEFT	) { bControll = true; m_Unit->Input(CUnit::InputKey::MoveLeft, bPushShift);		}
				if(dwDirection & DIR_RIGHT	) { bControll = true; m_Unit->Input(CUnit::InputKey::MoveRight, bPushShift);	}
			}

		}
		
		if (bControll == false)
			m_Unit->Input(CUnit::InputKey::KeyUp);

		return bControll;
	}
}