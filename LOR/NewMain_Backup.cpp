//----------------------
// 게임공학과 13학번(2013182027) 원성연
// 네트워크 게임 프로그래밍 - Lege of Runners
// 2017-09-27
//----------------------

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif

#include "stdafx.h"
#include "ReplayFunction.h"

#include "StaticActor.h"
#include "Pawn.h"
#include "Map.h"

#pragma region [SoundManger]
System*			pSystem;

//	사운드의 개수만큼 필요
Sound*			pSound[8];

//	동시에 출력될 사운드 만큼 필요
Channel*		pChannel[1];

#pragma endregion

#define MAX_FPS 60

#pragma region [CreateStatic]
HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "Network Game - project [ 2013192027 : 원성연 ]";

static int GAME_MODE;					//게임의 모드 설정!!  0 -> 1 -> 2 -> 3

LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);

bool KeyBuffer[256];

std::chrono::system_clock::time_point m_current_time;
std::chrono::duration<double> m_timeElapsed; // 시간이 얼마나 지났나
double m_fps;

#pragma endregion

#pragma region [WinMain]
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;
	MSG Message;
	WNDCLASSEX WndClass;
	hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground =
		(HBRUSH)GetStockObject(WHITE_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);
	hwnd = CreateWindow(lpszClass, lpszClass,
		WS_OVERLAPPEDWINDOW || WS_BORDER,
		100, 30,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		NULL, (HMENU)NULL,
		hInstance, NULL);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	memset(&Message, 0, sizeof(Message));

	//while (GetMessage(&Message, 0, 0, 0))
	//{
	//	TranslateMessage(&Message);
	//	DispatchMessage(&Message);
	//}
	//return Message.wParam;
	while (WM_QUIT != Message.message)
	{
		if (PeekMessage(&Message, hwnd, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else
		{
			m_timeElapsed = chrono::system_clock::now() - m_current_time;
			if (m_timeElapsed.count() > MAX_FPS)
			{
				m_current_time = chrono::system_clock::now();

				if (m_timeElapsed.count() > 0.0)
					m_fps = 1.0 / m_timeElapsed.count();

				InvalidateRect(hwnd, NULL, FALSE);

#if defined(SHOW_CAPTIONFPS)

				m_UpdateElapsed = chrono::system_clock::now() - m_LastUpdate_time;
				if (m_UpdateElapsed.count() > MAX_UPDATE_FPS)
					m_LastUpdate_time = chrono::system_clock::now();
				else return;

				_itow_s(m_fps + 0.1f, m_CaptionTitle + m_TitleLength, TITLE_MX_LENGTH - m_TitleLength, 10);
				wcscat_s(m_CaptionTitle + m_TitleLength, TITLE_MX_LENGTH - m_TitleLength, TEXT(" FPS)"));
				SetWindowText(hwnd, m_CaptionTitle);
#endif
			}
			// 최대 FPS 미만의 시간이 경과하면 진행 생략
		}
	}

	return Message.wParam;
}
#pragma endregion

LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam)
{
	srand((unsigned)time(NULL));

	HDC hdc, Memdc;
	PAINTSTRUCT ps;
	//HBITMAP hBitmap, OldBitmap;
	//HFONT hFont, saveFont;
	//HBRUSH Brush, oldBrush;
	//HPEN MyPen, OldPen, RedPen;

	static RECT rect;

	static Map map(0, 0, "Resource/Image/Background/Background.png");
	static StaticActor grid(0, 0, "Resource/Image/grid.png");
	static bool isDrawGrid = true;

	static Pawn myPawn(CharacterName::Archer);

	switch (iMessage) {
	case WM_CREATE:
	{
		GetClientRect(hwnd, &rect);
		SetTimer(hwnd, 1, 1, NULL);

		break;
	}
	case WM_PAINT:
	{
		HDC mainHDC = BeginPaint(hwnd, &ps);
#pragma region [Doubble buffer]
		HBITMAP GLay = CreateCompatibleBitmap(mainHDC, SCREEN_WIDTH, SCREEN_HEIGHT);
		HDC hdc = CreateCompatibleDC(mainHDC);
		SelectObject(hdc, GLay);
		Rectangle(hdc, -10, -10, SCREEN_WIDTH + 20, SCREEN_HEIGHT + 20);
#pragma endregion
		SetStretchBltMode(mainHDC, COLORONCOLOR);
		SetStretchBltMode(hdc, COLORONCOLOR);

		map.Draw(hdc);
		grid.Draw(hdc, TRUE, isDrawGrid);

		myPawn.Draw(hdc);

		SetStretchBltMode(mainHDC, COLORONCOLOR);
		SetStretchBltMode(hdc, COLORONCOLOR);
#pragma region [Doubble buffer]
		BitBlt(mainHDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

		//DeleteDC(Memdc);
		DeleteDC(hdc);
		DeleteObject(GLay);
		EndPaint(hwnd, &ps);
#pragma endregion
		break;
	}
	case WM_TIMER:
	{
		myPawn.Update(myPawn.GetState());
		map.Update(myPawn.GetSpeed());
		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
	{
		myPawn.InsertKey(wParam);

		if (wParam == 'g' || wParam == 'G') {
			if (isDrawGrid) isDrawGrid = false;
			else isDrawGrid = true;
		}
		break;
	}
	case WM_KEYUP:
	{
		break;
	}
	case WM_LBUTTONDOWN:
	{
		break;
	}
	case WM_RBUTTONDOWN:
	{
		break;
	}
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}
	}
	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
}
