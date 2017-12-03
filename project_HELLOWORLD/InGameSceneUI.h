#pragma once

#include "stdafx.h"
#include "BaseUI.h"

class InGameSceneUI : public BaseUI
{
public:
	InGameSceneUI();
	~InGameSceneUI();

private:
	CImage		ComboNumUI[10];
	CImage		ComboTextUI;

	CImage		BarUI;
	CImage		BarMarkUI[4];

	CImage		InventoryUI;
	CImage		ItemUI[4];

	CImage		HeadUpUI[1];
	int			m_headUpUICount{ 0 };
	int			m_headUpUIIndex{ 0 };

	CImage		EmotionUI;

public:
	void LoadUI();

public:
	void DrawComboUI(HDC hdc, const int combo);
	void DrawBarUI(HDC hdc, int idx, float markPosX);
	void DrawInventoryUI(HDC hdc, const int item1Num, const int item2Num);
	void DrawHeadUpUI(HDC, const float posY, float playerDisX, float thisDisX);
	void DrawPlayerMark(HDC hdc, int idx, float posY, float playerDisX = 0, float thisDisX = 0);
	void DrawEmotionUI(HDC hdc, const int keyNumber, float posX, float posY);
	void DrawEmotionUI(HDC hdc, const int keyNumber, float playerDisX, float thisDisX, float posY);
};

