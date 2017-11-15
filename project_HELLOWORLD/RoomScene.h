#pragma once

#include "Scene.h"

class RoomScene : public Scene
{
public:
	RoomScene();
	RoomScene(HWND);
	~RoomScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);

	virtual bool MouseProcess(HWND hwnd, UINT iMessage, WPARAM wParam, LPARAM lParam);


public:
	void LoadCImage();
	void DrawCharacter(HDC hdc);

private:
	CImage			m_backImg[2];
	CImage			m_ImgArcher;
	CImage			m_ImgZombie;
	CImage			m_ImgKnight;
	CImage			m_ImgWicher;


	MapName			m_What_Is_Your_Map{ MapName::Sea };
	CharacterName	m_characterName{ CharacterName::Archer };


public:
	virtual MapName		GetMapName() { return m_What_Is_Your_Map; }
	virtual CharacterName GetCharacterName() { return m_characterName; }
};

