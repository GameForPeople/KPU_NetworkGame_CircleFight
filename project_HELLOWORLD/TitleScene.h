#pragma once
#include "Scene.h"

class TitleScene : 
	public Scene
{

private:
	//CImage			m_titleImage;
	CImage				m_blackScreen;

	//�̹��� DC
	HDC				m_ImageDC;
	HBITMAP			MyImgBit, OldImgBit;
	POINT			m_ptImgSize;

	//���ĺ��� ���� ����
	BLENDFUNCTION	m_BlendFution;
	int				m_nAlpha;
	int				m_fullCount;
	bool			m_isOnUpAlpha;

public:
	TitleScene();
	TitleScene(HWND);
	virtual ~TitleScene();

public:
	virtual void Draw(HDC hdc);
	virtual void Timer(const double count);

public:
	void LoadCImage();
	void SetUp();
};

