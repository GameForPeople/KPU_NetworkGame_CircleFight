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

	HeadUpUI[0].Load("Resource/Image/UI/SpeedUpUI.png");
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

void InGameSceneUI::DrawBarUI(HDC hdc, float mark1PosX) {
	BarUI.TransparentBlt(hdc, 200, 10, BarUI.GetWidth() - 50, BarUI.GetHeight(), RGB(255, 255, 255));

	BarMarkUI[0].TransparentBlt(hdc, mark1PosX + 200 , 20, BarMarkUI[0].GetWidth() - 20, BarMarkUI[0].GetHeight() - 20, RGB(255, 255, 255));
}

void InGameSceneUI::DrawInventoryUI(HDC hdc, int item1Num, int item2Num) {
	InventoryUI.TransparentBlt(hdc, 10, 550, 220, 120, RGB(255, 255, 255));
}

void InGameSceneUI::DrawHeadUpUI(HDC hdc, float posY) {
	if (m_headUpUIIndex) {
		m_headUpUICount++;

		HeadUpUI[m_headUpUIIndex - 1].TransparentBlt(hdc, 310, posY - 70, 120, 70, RGB(255, 255, 255));
		if (m_headUpUICount == 80) {
			m_headUpUICount = 0;
			m_headUpUIIndex = 0;
		}
	}
}