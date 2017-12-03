#include "InGameSceneUI.h"

InGameSceneUI::InGameSceneUI()
{
	LoadUI();
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

	EmotionUI.Load("Resource/Image/UI/EmotionUI.png");
}




void InGameSceneUI::DrawComboUI(HDC hdc, int combo) {	
	
	int sizeBuffer = 0;

	if (combo % 50 == 0) {
		sizeBuffer = 30;
		m_headUpUIIndex = 1;
	}

	if (combo >= 100) {
		ComboNumUI[combo / 100].TransparentBlt(hdc, 10 - sizeBuffer , 100 - sizeBuffer / 2, 58 + sizeBuffer, 85 + sizeBuffer, RGB(255, 255, 255));
	}

	if (combo >= 10) {
		ComboNumUI[(combo % 100)/ 10].TransparentBlt(hdc, 60 , 100 - sizeBuffer / 2 , 58 , 85 + sizeBuffer, RGB(255, 255, 255));
	}

	ComboNumUI[combo % 10].TransparentBlt(hdc, 110 , 100 - sizeBuffer / 2 , 58 + sizeBuffer, 85 + sizeBuffer , RGB(255,255,255));

	ComboTextUI.TransparentBlt(hdc, 160 + sizeBuffer, 140 - sizeBuffer / 2, ComboTextUI.GetWidth() + sizeBuffer, ComboTextUI.GetHeight() + sizeBuffer, RGB(255,255,255));
}

void InGameSceneUI::DrawBarUI(HDC hdc, int idx, float markPosX) {
	BarUI.TransparentBlt(hdc, 200, 10, BarUI.GetWidth() - 50, BarUI.GetHeight(), RGB(255, 255, 255));

	BarMarkUI[idx].TransparentBlt(hdc, markPosX + 200 , 20, BarMarkUI[idx].GetWidth() - 20, BarMarkUI[idx].GetHeight() - 20, RGB(255, 255, 255));
}

void InGameSceneUI::DrawInventoryUI(HDC hdc, int item1Num, int item2Num) {
	InventoryUI.TransparentBlt(hdc, 10, 550, 220, 120, RGB(255, 255, 255));
	if (item1Num != -1)
	{
		ItemUI[item1Num].TransparentBlt(hdc, 20, 560, 90, 90, RGB(255, 255, 255));
	}
	if (item2Num != -1)
	{
		ItemUI[item2Num].TransparentBlt(hdc, 130, 560, 90, 90, RGB(255, 255, 255));
	}
}

void InGameSceneUI::DrawHeadUpUI(HDC hdc, float posY, float playerDisX, float thisDisX) {
	if (thisDisX - playerDisX > 1100) return;
	if (playerDisX - thisDisX > 400) return;

	float newPosX = thisDisX - playerDisX;

	if (m_headUpUIIndex) {
		m_headUpUICount++;

		HeadUpUI[m_headUpUIIndex - 1].TransparentBlt(hdc, newPosX + 310, posY - 70, 120, 70, RGB(255, 255, 255));
		if (m_headUpUICount == 80) {
			m_headUpUICount = 0;
			m_headUpUIIndex = 0;
		}
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

			(EmotionUI.GetWidth()* (keyNumber - 1)) / 6 ,
			0, 
			EmotionUI.GetWidth() / 6 , 
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
		newPosX +320,
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