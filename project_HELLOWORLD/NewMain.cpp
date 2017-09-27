//----------------------
// 게임공학과 13학번(2013182027) 원성연
// 네트워크 게임 프로그래밍 - Lege of Runners
// 2017-09-27
//----------------------


#include "stdafx.h"
#include "ReplayFunction.h"

#include "StaticActor.h"

#pragma region [SoundManger]
System*			pSystem;

//	사운드의 개수만큼 필요
Sound*			pSound[8];

//	동시에 출력될 사운드 만큼 필요
Channel*		pChannel[1];

#pragma endregion

#pragma region [CreateStatic]
HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "Network Game - project [ 2013192027 : 원성연 ]";

static int GAME_MODE;					//게임의 모드 설정!!  0 -> 1 -> 2 -> 3

LRESULT CALLBACK WndProc(HWND hwnd, UINT
	iMessage, WPARAM wParam, LPARAM lParam);

bool KeyBuffer[256];

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
	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
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

	static StaticActor map(0, 0, "Resource/Image/backImg.png");
	static StaticActor grid(0, 0, "Resource/Image/grid.png");
	static bool isDrawGrid = true;

	switch (iMessage) {
	case WM_CREATE:
	{
		GetClientRect(hwnd, &rect);
		SetTimer(hwnd, 1, 40, NULL);

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

		map.Draw(hdc, TRUE);
		grid.Draw(hdc, TRUE, isDrawGrid);

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
		InvalidateRgn(hwnd, NULL, false);
		break;
	}
	case WM_KEYDOWN:
	{
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
