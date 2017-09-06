#include "stdafx.h"
#include <Windows.h>
#include <iostream>

#include "Framework.h"
#include "TitleScene.h"

using namespace std;

CTitleScene::CTitleScene()
{

}


CTitleScene::~CTitleScene()
{
}

void CTitleScene::Update(float fTimeElapsed)
{
}

void CTitleScene::Draw(HDC hdc)
{
	m_cimgBackground.StretchBlt(hdc, m_rcScene);
}

bool CTitleScene::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		m_pFramework->ChangeScene(CurrentScene::Lobby);
		return true;
	case WM_KEYUP:
		return true;
	}
	return false;
}

bool CTitleScene::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	case WM_LBUTTONDOWN:
		m_pFramework->ChangeScene(CurrentScene::Lobby);
		return true;
	case WM_RBUTTONDOWN:
		break;
	case WM_MOUSEMOVE:
		return true;
	}
	return false;
}

bool CTitleScene::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
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

void CTitleScene::BuildObjects(CFramework * myFramework, HWND hWnd, CurrentScene tag)
{
	CScene::BuildObjects(myFramework, hWnd, tag);
	m_cimgBackground.Load(TEXT("Resource\\Graphics\\Background\\BG_Title.png"));
}

void CTitleScene::ReleaseObjects()
{
}