#pragma once
#include "Scene.h"

#define BUF_MAX_SIZE 30
#define BUF_SIZE 20

struct RoomData {
	int playersNumber{};
	int mapNumber{};
};

class LobbyScene :
	public Scene
{
public:
	LobbyScene();
	LobbyScene(HWND);
	LobbyScene(HWND, Network&);
	LobbyScene(HWND, Network*);

	~LobbyScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);
	
	virtual bool KeyProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);
	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


public:
	void LoadCImage();


private:
	CImage		m_backImg;

	CImage		m_roomImg[4];

	TCHAR		m_chatBuf[5][BUF_MAX_SIZE] = { NULL };
	TCHAR		m_chat[BUF_SIZE] = { NULL };
	size_t		m_Len = 0;

public:
	RoomData	m_roomData[8];
};

