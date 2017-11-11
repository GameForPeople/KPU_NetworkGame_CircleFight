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

	CImage		HeadUpUI[1];
	int			m_headUpUICount{ 0 };
	int			m_headUpUIIndex{ 0 };

public:
	void LoadUI();

public:
	void DrawComboUI(HDC hdc, const int combo);
	void DrawBarUI(HDC hdc, float mark1PosX);
	void DrawInventoryUI(HDC hdc, const int item1Num, const int item2Num);
	void DrawHeadUpUI(HDC, const float posY);
};
