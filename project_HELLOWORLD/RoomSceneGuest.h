#pragma once

#include "Scene.h"

class RoomSceneGuest : public Scene
{
public:
	RoomSceneGuest();
	RoomSceneGuest(HWND);
	~RoomSceneGuest();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);

	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


public:
	void LoadCImage();
	void DrawCharacter(HDC hdc);
	int GetXPosByIdx(int idx);
	int GetYPosByIdx(int idx);

private:
	CImage			m_backImg[2];
	CImage			m_ImgArcher;
	CImage			m_ImgZombie;
	CImage			m_ImgKnight;
	CImage			m_ImgWicher;


	MapName			m_What_Is_Your_Map{ MapName::Sea };
	CharacterName	m_characterName{ CharacterName::Archer };

	RoomInfo roomInfo;

public:
	virtual MapName		GetMapName() { return m_What_Is_Your_Map; }
	virtual CharacterName GetCharacterName() { return m_characterName; }
};