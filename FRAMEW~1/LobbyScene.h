#pragma once
#include "Scene.h"
#include <atlimage.h>

class CLobbyScene :public CScene
{
protected:

	CImage m_cimgBackground;
	CUnit			*m_Unit;

public:
	CLobbyScene();
	~CLobbyScene();

	virtual void Update(float fTimeElapsed); 
	virtual void Draw(HDC hdc);

	// 함수 호출 이후에도 처리를 원하면 false 반환
	virtual bool OnProcessingKeyboardMessage	(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingMouseMessage		(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	virtual bool OnProcessingWindowMessage		(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	virtual void BuildObjects(CFramework* pFramework, HWND hwnd, CurrentScene tag);
	virtual void ReleaseObjects();

	void UnitInitialize();
	bool ProcessInput();
};
