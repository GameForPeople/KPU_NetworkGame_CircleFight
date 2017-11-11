#include "TitleScene.h"



TitleScene::TitleScene()
{
}

TitleScene::TitleScene(HWND hWnd) : Scene(hWnd)
{
	LoadCImage();
	SetUp();
}


TitleScene::~TitleScene()
{
	//��Ʈ�� �����ϱ�
	DeleteObject(SelectObject(m_ImageDC, OldImgBit));

	//DC ���� �ϱ�
	DeleteDC(m_ImageDC);
}

void TitleScene::SetUp()
{
	//���� DC������
	HDC hdc = GetDC(m_hWnd);

	//�̹��� ũ��

	//���� DC�� ������ ������ DC �����
	m_ImageDC = CreateCompatibleDC(hdc);

	//�̹��� �ҷ����� (���ϰ�η� ���� �ε� )
	MyImgBit = (HBITMAP)LoadImage(NULL, "Resource/Image/Title/Title.bmp",
		IMAGE_BITMAP, SCREEN_WIDTH, SCREEN_HEIGHT, LR_LOADFROMFILE);

	//�̹����� DC�� �����ϱ�
	OldImgBit = (HBITMAP)SelectObject(m_ImageDC, MyImgBit);

	//���� DC �����ϱ�
	ReleaseDC(m_hWnd, hdc);

	//���İ��� 0���� �����ؼ� ����
	m_nAlpha = 1;
	m_isOnUpAlpha = true;
	m_fullCount = 0;
	m_isDestory = false;
}

void TitleScene::Draw(HDC hdc) {
	
	m_blackScreen.BitBlt(hdc, 0, 0, SRCCOPY);
	/*
	HDC AlphaDC = CreateCompatibleDC(hdc);

	HBITMAP TempDC = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
	HBITMAP OldTempDC = (HBITMAP)SelectObject(AlphaDC, TempDC);
	//m_titleImage.BitBlt(hdc, 0, 0, SRCCOPY);
	BitBlt(AlphaDC, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hdc, 0, 0, SRCCOPY);

	m_titleImage.AlphaBlend(AlphaDC, SCREEN_WIDTH, SCREEN_HEIGHT, m_timer, AC_SRC_ALPHA);

	DeleteObject(SelectObject(AlphaDC, OldTempDC));
	DeleteDC(AlphaDC);

	BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, hDC, 0, 0, SRCCOPY);
	*/

	//�� 3������ ���ؼ��� ���� ������..�ϴ� �̴�� ����
	m_BlendFution.BlendOp = AC_SRC_OVER;
	m_BlendFution.BlendFlags = 0;
	m_BlendFution.AlphaFormat = 0;

	//���İ� ( 0 - 255 ) ���� Ŭ���� �����ϴ�.
	m_BlendFution.SourceConstantAlpha = m_nAlpha;


	//���ĺ��� �Լ��̴�. 
	AlphaBlend(hdc,			//����� ���� DC
		0,					//����� DC���� �׸��� ���� ��ǥ x
		0,					//����� DC���� �׸��� ���� ��ǥ y
		SCREEN_WIDTH,		//����� DC���� �׸��� ���� ����
		SCREEN_HEIGHT,		//����� DC���� �׸��� ���� ����
		m_ImageDC,			//�׸��� DC
		0,					//�׸����� �׸��� ������ ��ǥ x
		0,					//�׸����� �׸��� ������ ��ǥ y
		SCREEN_WIDTH,		//�׸����� ���DC�� �׸� �׸��� ���� ����
		SCREEN_HEIGHT,		//�׸����� ���DC�� �׸� �׸��� ���� ����
		m_BlendFution);	//���ĺ��忡���� �ɼǰ�
}

void TitleScene::Timer(const double count) {
	//���İ��� ���� ���ؼ� �����ϰ� �����.
	
	if (m_isOnUpAlpha)
		m_nAlpha+= 3;
	else
		m_nAlpha-= 5;

	//�Ѱ��ġ�� ������� 0���� �ʱ�ȭ �Ѵ�.
	if (m_nAlpha >= 255)
	{
		m_nAlpha = 255;
		m_fullCount++;

		if(m_fullCount >= 50)
		m_isOnUpAlpha = false;
	}
	else if (m_nAlpha <= 0) {
		m_nAlpha = 0;
		m_fullCount--;
		if (m_fullCount <= 20) {
			m_nextScene = SceneName::Login;
			m_isDestory = true;
		}
	}
}

void TitleScene::LoadCImage() {
	//m_titleImage.Load("Resource/Image/Title/Title.png");b
	m_blackScreen.Load("Resource/Image/Title/BlackScreen.png");
}
