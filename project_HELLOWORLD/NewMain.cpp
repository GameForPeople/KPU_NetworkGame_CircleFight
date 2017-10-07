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

#include "Framework.h"

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

//LRESULT CALLBACK WndProc(HWND hwnd, UINT
//	iMessage, WPARAM wParam, LPARAM lParam);

Framework myFramework;

bool KeyBuffer[256];

void gotoMessageProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam);

#pragma endregion

#pragma region [WinMain]
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE
	hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hwnd;

	hInst = hInstance;

	DWORD dwStyle =
		WS_OVERLAPPED 	// 디폴트 윈도우. 타이틀 바와 크기 조절이 안되는 경계선을 가진다. 아무런 스타일도 주지 않으면 이 스타일이 적용된다.
		| WS_CAPTION 		// 타이틀 바를 가진 윈도우를 만들며 WS_BORDER 스타일을 포함한다.
		| WS_SYSMENU		// 시스템 메뉴를 가진 윈도우를 만든다.
		| WS_MINIMIZEBOX	// 최소화 버튼을 만든다.
		| WS_BORDER			// 단선으로 된 경계선을 만들며 크기 조정은 할 수 없다.
							//		| WS_THICKFRAME		// 크기 조정이 가능한 두꺼운 경계선을 가진다. WS_BORDER와 같이 사용할 수 없다.
		;					// 추가로 필요한 윈도우 스타일은 http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm 참고.

	RECT getWinSize;	// 윈도위의 크기를 받아옴!
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	RECT rc;
	rc.left = rc.top = 0;
	rc.right = SCREEN_WIDTH;
	rc.bottom = SCREEN_HEIGHT;

	AdjustWindowRect(&rc, dwStyle, FALSE);	//윈도우의 보정되는 것을 조절

											//	클라이언트 절대좌표(left, top)
											//	데스크톱의 중앙에 클라이언트가 위치하도록 설정
	POINT ptClientWorld;
	ptClientWorld.x = (getWinSize.right - SCREEN_WIDTH) / 2;
	ptClientWorld.y = (getWinSize.bottom - SCREEN_HEIGHT) / 2;

	MSG Message;
	WNDCLASSEX WndClass;

#pragma region [Hello! My Name is Dirty Code!]
	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)gotoMessageProc;
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
	
	hwnd = CreateWindow(
		lpszClass
		, lpszClass
		, dwStyle
		, ptClientWorld.x		
		, ptClientWorld.y		
		, SCREEN_WIDTH
		, SCREEN_HEIGHT
		, NULL
		, (HMENU)NULL
		, hInstance
		, NULL
	);

#pragma endregion
	memset(&Message, 0, sizeof(Message));

	myFramework.Create(hwnd, rc);
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	while (true)
	{	
		if (PeekMessage(&Message, nullptr, 0, 0, PM_REMOVE)) 
		{
			//std::cout << "Hello!" << std::endl;

			if (Message.message == WM_QUIT)	break;

			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}
		else myFramework.Timer();	
	}

	return Message.wParam;
}
#pragma endregion

void gotoMessageProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam) {
	myFramework.MessageProc(hWnd, nMessageID, wParam, lParam);
}

//LRESULT CALLBACK WndProc(HWND hwnd, UINT
//	iMessage, WPARAM wParam, LPARAM lParam)
//{
//
//	HDC hdc, Memdc;
//	PAINTSTRUCT ps;
//	//HBITMAP hBitmap, OldBitmap;
//	//HFONT hFont, saveFont;
//	//HBRUSH Brush, oldBrush;
//	//HPEN MyPen, OldPen, RedPen;
//
//	static RECT rect;
//
//	static Map map(0, 0, "Resource/Image/Background/Background.png");
//	static StaticActor grid(0, 0, "Resource/Image/grid.png");
//	static bool isDrawGrid = true;
//
//	static Pawn myPawn(CharacterName::Archer);
//
//	return(DefWindowProc(hwnd, iMessage, wParam, lParam));
//}
