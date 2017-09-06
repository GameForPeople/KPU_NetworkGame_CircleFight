#include "stdafx.h"
#include "Framework.h"
#include <string>

#include "SpriteImage.h"
#include "Unit.h"

#include "TitleScene.h"
#include "LobbyScene.h"

using namespace std;

template <typename T>
T GetUserDataPtr(HWND hWnd) 
{
	return reinterpret_cast<T>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
}

void SetUserDataPtr(HWND hWnd, LPVOID ptr) 
{
	LONG_PTR result = ::SetWindowLongPtr(
		hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(ptr));

}

CFramework::CFramework()
{
}

CFramework::~CFramework()
{
	OnDestroy();
}

bool CFramework::OnCreate(HWND hWnd, const RECT & rc)
{
	m_hWnd = hWnd;
	m_rcClient = rc;

	// Ŭ���̾�Ʈ ��ǥ �ʱ�ȭ
	m_rcClient.right -= m_rcClient.left;
	m_rcClient.bottom -= m_rcClient.top;
	m_rcClient.left = 0;
	m_rcClient.top = 0;

	// ���� ����
	CreatebackBuffer();

	// Ŭ������ ������ ���ν��� ����
	::SetUserDataPtr(m_hWnd, this);

	// ���� Scene�� ���°ɷ�
	m_pCurrentScene = nullptr;
	// Scene �迭 �ʱ�ȭ
	for (auto& p : m_arrScene) 
		p = nullptr;

	// Scene ����
	BuildScene();

	// ĸ�� ����
	lstrcpy(m_CaptionTitle, TITLESTRING);

#if defined(SHOW_CAPTIONFPS)
	lstrcat(m_CaptionTitle, TEXT(" ("));
#endif
	m_TitleLength = lstrlen(m_CaptionTitle);
	SetWindowText(m_hWnd, m_CaptionTitle);
	
	// Ÿ�̸� �ʱ�ȭ
	m_LastUpdate_time = chrono::system_clock::now();
	m_current_time = chrono::system_clock::now();
	m_fps = 0;

	return (m_hWnd != NULL);
}

void CFramework::CreatebackBuffer()
{
	if (m_hDC)
	{
		::SelectObject(m_hDC, NULL);
		::DeleteDC(m_hDC);
	}
	if (m_hBitmapBackBuffer) ::DeleteObject(m_hBitmapBackBuffer);

	HDC hdc = ::GetDC(m_hWnd);
	m_hDC = ::CreateCompatibleDC(hdc);
	m_hBitmapBackBuffer = ::CreateCompatibleBitmap(hdc, m_rcClient.right, m_rcClient.bottom);
	::SelectObject(m_hDC, m_hBitmapBackBuffer);

	SetBKColor(RGB(255, 255, 255));

	ClearBackgroundColor();

	::ReleaseDC(m_hWnd, hdc);
}

void CFramework::BuildScene()
{
	m_arrScene[GetSceneEnumInt(Title)] = new CTitleScene();
	m_arrScene[GetSceneEnumInt(Title)]->BuildObjects(this, m_hWnd, CScene::CurrentScene::Title);

	m_arrScene[GetSceneEnumInt(Lobby)] = new CLobbyScene();
	m_arrScene[GetSceneEnumInt(Lobby)]->BuildObjects(this, m_hWnd, CScene::CurrentScene::Lobby);

	m_pCurrentScene = m_arrScene[GetSceneEnumInt(Title)]; // ������� Ÿ��Ʋ�� �ʱ�ȭ

}

bool CFramework::OnDestroy()
{
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);
	
	::SelectObject(m_hDC, NULL);
	if (m_hBitmapBackBuffer) ::DeleteObject(m_hBitmapBackBuffer);
	if (m_hDC) ::DeleteDC(m_hDC);

	return false;
}

bool CFramework::OnProcessingKeyboardMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pCurrentScene->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam)) return false;
	switch (nMessageID)
	{
	case WM_KEYDOWN:
		return true;
	case WM_KEYUP:
		switch (wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		return true;
	}
	return false;
}

bool CFramework::OnProcessingMouseMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	if (m_pCurrentScene->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam)) return false;
	switch (nMessageID)
	{
	case WM_LBUTTONUP:
		break;

	case WM_LBUTTONDOWN:
		break;

	case WM_MOUSEMOVE:
		break;
	}
	return false;
}

HRESULT CFramework::OnProcessingWindowMessage(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	switch (nMessageID)
	{
	default:
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);

	}
	return 0;
}

void CFramework::Update(float fTimeElapsed)
{
	m_pCurrentScene->Update(fTimeElapsed);
}

void CFramework::SetBKColor(COLORREF color)
{
	m_clrBackBuffer = color;
	if (m_hbrBackground) ::DeleteObject(m_hbrBackground);
	m_hbrBackground = ::CreateSolidBrush(m_clrBackBuffer);
}

void CFramework::ClearBackgroundColor()
{
	::FillRect(m_hDC, &m_rcClient, m_hbrBackground);
}

void CFramework::PreproccessingForDraw()
{
	ClearBackgroundColor();
	::SetBkMode(m_hDC, TRANSPARENT);
	::SetStretchBltMode(m_hDC, COLORONCOLOR);

	m_pCurrentScene->Draw(m_hDC);
}

void CFramework::OnDraw(HDC hDC)
{
	::BitBlt(hDC, m_rcClient.left, m_rcClient.top, m_rcClient.right, m_rcClient.bottom, m_hDC, 0, 0, SRCCOPY);
}

void CFramework::ChangeScene(CScene::CurrentScene Tag, bool bDestroy)
{
	const int tag = GetEnumValueByType(Tag);
	if (bDestroy) {
		int d_tag = GetEnumValueByType(m_pCurrentScene->GetMyTag());
		delete m_pCurrentScene;
		m_pCurrentScene = nullptr;
		m_arrScene[d_tag] = nullptr;
	}
	if (m_arrScene[tag]) { // ���� nullptr�� �ƴ� ���
		m_pCurrentScene = m_arrScene[tag];
	}
	else {
		switch (Tag) {
		case CScene::CurrentScene::Title:
			m_arrScene[GetSceneEnumInt(Title)] = new CTitleScene();
			m_arrScene[GetSceneEnumInt(Title)]->BuildObjects(this, m_hWnd, CScene::CurrentScene::Title);
			break;
		case CScene::CurrentScene::Lobby:
			m_arrScene[GetSceneEnumInt(Lobby)] = new CLobbyScene();
			m_arrScene[GetSceneEnumInt(Lobby)]->BuildObjects(this, m_hWnd, CScene::CurrentScene::Lobby);
			break;
		}
		m_pCurrentScene = m_arrScene[tag];
	}
}

void CFramework::FrameAdvance()
{
	// Get tick
	m_timeElapsed = chrono::system_clock::now() - m_current_time;
	if (m_timeElapsed.count() > MAX_FPS)
	{
		m_current_time = chrono::system_clock::now();
	
		if (m_timeElapsed.count() > 0.0)
			m_fps = 1.0 / m_timeElapsed.count();
	}
	// �ִ� FPS �̸��� �ð��� ����ϸ� ���� ����
	else return;

	Update(m_timeElapsed.count());
	PreproccessingForDraw();

	InvalidateRect(m_hWnd, &m_rcClient, FALSE);

#if defined(SHOW_CAPTIONFPS)

	m_UpdateElapsed = chrono::system_clock::now() - m_LastUpdate_time;
	if (m_UpdateElapsed.count() > MAX_UPDATE_FPS)
		m_LastUpdate_time = chrono::system_clock::now();
	else return;

	_itow_s(m_fps + 0.1f, m_CaptionTitle + m_TitleLength, TITLE_MX_LENGTH - m_TitleLength, 10);
	wcscat_s(m_CaptionTitle + m_TitleLength, TITLE_MX_LENGTH - m_TitleLength, TEXT(" FPS)"));
	SetWindowText(m_hWnd, m_CaptionTitle);
#endif

}

LRESULT CFramework::WndProc(HWND hWnd, UINT nMessageID, WPARAM wParam, LPARAM lParam)
{
	CFramework* self = ::GetUserDataPtr<CFramework*>(hWnd);
	if (!self)
		return ::DefWindowProc(hWnd, nMessageID, wParam, lParam);

	switch (nMessageID)
	{

	case WM_LBUTTONDOWN:
	case WM_LBUTTONUP:

	case WM_RBUTTONDOWN:
	case WM_RBUTTONUP:

	case WM_MOUSEMOVE:
		self->OnProcessingMouseMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_KEYDOWN:
	case WM_KEYUP:
		self->OnProcessingKeyboardMessage(hWnd, nMessageID, wParam, lParam);
		break;

	case WM_PAINT:
		{
			PAINTSTRUCT ps;
			HDC hdc = ::BeginPaint(hWnd, &ps);
			
			self->OnDraw(hdc);

			::EndPaint(hWnd, &ps);
		}
		break;

	case WM_DESTROY:
		::SetUserDataPtr(hWnd, NULL);
		::PostQuitMessage(0);
		break;

	default:
		return self->OnProcessingWindowMessage(hWnd, nMessageID, wParam, lParam);

	}
	return 0;
}
