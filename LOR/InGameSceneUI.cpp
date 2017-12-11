#include "InGameSceneUI.h"

InGameSceneUI::InGameSceneUI()
{
	LoadUI();
	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		setItemKind[i] = -1;
		resetItemKind[i] = -1;
	}
}

InGameSceneUI::~InGameSceneUI()
{
}



void InGameSceneUI::LoadUI() {
	ComboNumUI[0].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_0.png");
	ComboNumUI[1].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_1.png");
	ComboNumUI[2].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_2.png");
	ComboNumUI[3].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_3.png");
	ComboNumUI[4].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_4.png");
	ComboNumUI[5].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_5.png");
	ComboNumUI[6].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_6.png");
	ComboNumUI[7].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_7.png");
	ComboNumUI[8].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_8.png");
	ComboNumUI[9].Load("Resource/Image/Number/ComboNumber/Combo_Orange_Yellow_9.png");

	ComboTextUI.Load("Resource/Image/Number/ComboNumber/Combo_Orange_type_1.png");

	BarUI.Load("Resource/Image/UI/BarUI_6.png");

	BarMarkUI[0].Load("Resource/Image/UI/BarUserMarkUI_1.png");
	BarMarkUI[1].Load("Resource/Image/UI/BarUserMarkUI_2.png");
	BarMarkUI[2].Load("Resource/Image/UI/BarUserMarkUI_3.png");
	BarMarkUI[3].Load("Resource/Image/UI/BarUserMarkUI_4.png");

	InventoryUI.Load("Resource/Image/UI/InventoryUI_type_4.png");

	ItemUI[0].Load("Resource/Image/UI/item_attack_1_only.png");
	ItemUI[1].Load("Resource/Image/UI/item_attack_2_only.png");
	ItemUI[2].Load("Resource/Image/UI/item_support_1_only.png");
	ItemUI[3].Load("Resource/Image/UI/item_support_2_only.png");

	HeadUpUI[0].Load("Resource/Image/UI/SpeedUpUI.png");
	HeadUpUI[1].Load("Resource/Image/UI/item_attack_1.png");
	HeadUpUI[2].Load("Resource/Image/UI/item_attack_2.png");
	HeadUpUI[3].Load("Resource/Image/UI/item_support_1.png");
	HeadUpUI[4].Load("Resource/Image/UI/item_support_2.png");

	EmotionUI.Load("Resource/Image/UI/EmotionUI.png");

	m_rankUI.Load("Resource/Image/UI/RankUI.png");
}


void InGameSceneUI::SetUI(int idx, int item)
{
	m_headUpUIIndex[idx] = item;
	m_headUpUICount[idx] = 0;
}

void InGameSceneUI::ResetUI(int idx, int item)
{
	if (m_headUpUIIndex[idx] != item) return;
	m_headUpUIIndex[idx] = -1;
}


void InGameSceneUI::DrawComboUI(HDC hdc, int combo) {

	int sizeBuffer = 0;

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (basicInfo.m_combo[i] % 20 == 0 && basicInfo.m_combo[i] > 10) {
			sizeBuffer = 20;
			m_headUpUIIndex[i] = 0;
		}
		if (setItemKind[i] != -1)
		{
			SetUI(i, setItemKind[i]); 
			setItemKind[i] = -1;
		}
		if (resetItemKind[i] != -1) 
		{
			ResetUI(i, resetItemKind[i]);
			resetItemKind[i] = -1;
		}
	}

	if (basicInfo.m_combo[m_idx] >= 100) {
		ComboNumUI[basicInfo.m_combo[m_idx] / 100].TransparentBlt(hdc, 10 - sizeBuffer, 100 - sizeBuffer / 2, 58 + sizeBuffer, 85 + sizeBuffer, RGB(255, 255, 255));
	}

	if (basicInfo.m_combo[m_idx] >= 10) {
		ComboNumUI[(basicInfo.m_combo[m_idx] % 100) / 10].TransparentBlt(hdc, 60, 100 - sizeBuffer / 2, 58, 85 + sizeBuffer, RGB(255, 255, 255));
	}

	ComboNumUI[basicInfo.m_combo[m_idx] % 10].TransparentBlt(hdc, 110, 100 - sizeBuffer / 2, 58 + sizeBuffer, 85 + sizeBuffer, RGB(255, 255, 255));

	ComboTextUI.TransparentBlt(hdc, 160 + sizeBuffer, 140 - sizeBuffer / 2, ComboTextUI.GetWidth() + sizeBuffer, ComboTextUI.GetHeight() + sizeBuffer, RGB(255, 255, 255));
}

void InGameSceneUI::DrawBarUI(HDC hdc, int idx, float markPosX) {
	BarUI.TransparentBlt(hdc, 200, 10, 1000, BarUI.GetHeight(), RGB(255, 255, 255));

	BarMarkUI[idx].TransparentBlt(hdc, markPosX + 200, 20, BarMarkUI[idx].GetWidth() - 20, BarMarkUI[idx].GetHeight() - 20, RGB(255, 255, 255));
}

void InGameSceneUI::DrawInventoryUI(HDC hdc, int item1Num, int item2Num) {
	InventoryUI.TransparentBlt(hdc, 10, 550, 220, 120, RGB(255, 255, 255));
	if (item1Num != -1)
	{
		ItemUI[item1Num - 1].TransparentBlt(hdc, 20, 560, 90, 90, RGB(255, 255, 255));
	}
	if (item2Num != -1)
	{
		ItemUI[item2Num - 1].TransparentBlt(hdc, 130, 560, 90, 90, RGB(255, 255, 255));
	}
}

void InGameSceneUI::DrawHeadUpUI(HDC hdc, float posY, float playerDisX, float thisDisX, int idx) {
	if (thisDisX - playerDisX > 1100) return;
	if (playerDisX - thisDisX > 400) return;

	float newPosX = thisDisX - playerDisX;

	if (m_headUpUICount[idx] > 80) {
		m_headUpUICount[idx] = 0;
		m_headUpUIIndex[idx] = -1;
	}

	if (m_headUpUIIndex[idx] != -1)
	{
		m_headUpUICount[idx]++;
		HeadUpUI[m_headUpUIIndex[idx]].TransparentBlt(hdc, newPosX + 345, posY - 70, 70, 70, RGB(255, 255, 255));
	}
}

void InGameSceneUI::DrawPlayerMark(HDC hdc, int idx, float posY, float playerDisX, float thisDisX)
{
	if (thisDisX - playerDisX > 1100) return;
	if (playerDisX - thisDisX > 400) return;

	float newPosX = thisDisX - playerDisX;

	BarMarkUI[idx].TransparentBlt(hdc, newPosX + 365, posY - 10, BarMarkUI[idx].GetWidth() - 40, BarMarkUI[idx].GetHeight() - 40, RGB(255, 255, 255));
}

void InGameSceneUI::DrawEmotionUI(HDC hdc, const int keyNumber, float posX, float posY) {
	EmotionUI.TransparentBlt(
		hdc,
		posX + 120,
		posY - 90,
		EmotionUI.GetWidth() / 6,
		EmotionUI.GetHeight(),

		(EmotionUI.GetWidth()* (keyNumber - 1)) / 6,
		0,
		EmotionUI.GetWidth() / 6,
		EmotionUI.GetHeight(),
		RGB(255, 255, 255)
	);
}

void InGameSceneUI::DrawEmotionUI(HDC hdc, const int keyNumber, float playerDisX, float thisDisX, float posY)
{
	if (thisDisX - playerDisX > 1100) return;
	if (playerDisX - thisDisX > 400) return;

	float newPosX = thisDisX - playerDisX;

	EmotionUI.TransparentBlt(
		hdc,
		newPosX + 320,
		posY - 90,
		EmotionUI.GetWidth() / 6,
		EmotionUI.GetHeight(),

		(EmotionUI.GetWidth()* (keyNumber - 1)) / 6,
		0,
		EmotionUI.GetWidth() / 6,
		EmotionUI.GetHeight(),
		RGB(255, 255, 255)
	);
}

void InGameSceneUI::DrawRankUI(HDC hdc)
{
	int myRanking = 0;

	for (int i = 0; i < MAX_PLAYER; ++i)
	{
		if (basicInfo.m_totalDis[i] > basicInfo.m_totalDis[m_idx])
			myRanking++;
	}

	m_rankUI.TransparentBlt(hdc, 40, 10, 130, 80, 128 * myRanking, 0, 128, 64, RGB(255, 255, 255));
}