//----------------------
// ���Ӱ��а� 13�й�(2013182027) ������
// ��Ʈ��ũ ���� ���α׷��� - Lege of Runners
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

//	������ ������ŭ �ʿ�
Sound*			pSound[8];

//	���ÿ� ��µ� ���� ��ŭ �ʿ�
Channel*		pChannel[1];

#pragma endregion

#pragma region [CreateStatic]
HINSTANCE hInst;
HWND	  Hwnd;
LPCTSTR lpszClass = "Network Game - project [ 2013192027 : ������ ]";

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
		WS_OVERLAPPED 	// ����Ʈ ������. Ÿ��Ʋ �ٿ� ũ�� ������ �ȵǴ� ��輱�� ������. �ƹ��� ��Ÿ�ϵ� ���� ������ �� ��Ÿ���� ����ȴ�.
		| WS_CAPTION 		// Ÿ��Ʋ �ٸ� ���� �����츦 ����� WS_BORDER ��Ÿ���� �����Ѵ�.
		| WS_SYSMENU		// �ý��� �޴��� ���� �����츦 �����.
		| WS_MINIMIZEBOX	// �ּ�ȭ ��ư�� �����.
		| WS_BORDER			// �ܼ����� �� ��輱�� ����� ũ�� ������ �� �� ����.
							//		| WS_THICKFRAME		// ũ�� ������ ������ �β��� ��輱�� ������. WS_BORDER�� ���� ����� �� ����.
		;					// �߰��� �ʿ��� ������ ��Ÿ���� http://www.soen.kr/lecture/win32api/reference/Function/CreateWindow.htm ����.

	RECT getWinSize;	// �������� ũ�⸦ �޾ƿ�!
	GetWindowRect(GetDesktopWindow(), &getWinSize);

	RECT rc;
	rc.left = rc.top = 0;
	rc.right = SCREEN_WIDTH;
	rc.bottom = SCREEN_HEIGHT;

	AdjustWindowRect(&rc, dwStyle, FALSE);	//�������� �����Ǵ� ���� ����

											//	Ŭ���̾�Ʈ ������ǥ(left, top)
											//	����ũ���� �߾ӿ� Ŭ���̾�Ʈ�� ��ġ�ϵ��� ����
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
