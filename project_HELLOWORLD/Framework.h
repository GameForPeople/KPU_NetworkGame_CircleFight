#pragma once

#include "stdafx.h"
#include "StaticActor.h"
#include "Scene.h"
#include "InGameScene.h"
#include "TitleScene.h"
#include "LoginScene.h"
#include "LobbyScene.h"
#include "RoomScene.h"
#include "RoomSceneGuest.h"
#include "InGameSceneGuest.h"

#define MAX_FPS  1.0 / 60.0
#define MAX_UPDATE_FPS  1.0 / 3.0
#define SCENE_NUM 5
#define TITLE_MX_LENGTH 64

class Framework
{
private:

	HWND			m_hwnd;
	RECT			m_rect{ 0,0,0,0 };

	HBITMAP			m_hBitmapBackBuffer = NULL;
	HDC				m_hDC = NULL;

	COLORREF		m_clrBackBuffer = 0x000000;
	HBRUSH			m_hbrBackground = NULL;

	MSG				m_msg;

	static int		GAME_MODE;


	std::chrono::system_clock::time_point m_current_time;
	std::chrono::duration<double> m_timeElapsed; // 시간이 얼마나 지났나
	double			m_fps;

	std::chrono::system_clock::time_point m_LastUpdate_time;
	std::chrono::duration<double> m_UpdateElapsed; // 시간이 얼마나 지났나
	TCHAR m_CaptionTitle[TITLE_MX_LENGTH];
	int m_TitleLength;

	Scene*			m_Scene[SCENE_NUM];
	//std::vector<Scene>	*m_Scene;

	int				m_nowScene = 0;

	//debug option
	StaticActor*	m_grid;
	bool			m_isGrid {false};

public:
	Framework();
	~Framework();

	bool Create(HWND hwnd, RECT rect);

	//static WNDPROC gotoMessageProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	HRESULT OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);
	LRESULT MessageProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

	void Draw(HDC hdc);
	void Timer();
	void Update(double);
	bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	void Destory();

	void ChangeScene();


	void CreatebackBuffer();
};

