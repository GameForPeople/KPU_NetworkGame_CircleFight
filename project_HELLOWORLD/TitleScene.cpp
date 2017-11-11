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
	//비트맵 삭제하기
	DeleteObject(SelectObject(m_ImageDC, OldImgBit));

	//DC 삭제 하기
	DeleteDC(m_ImageDC);
}

void TitleScene::SetUp()
{
	//메인 DC얻어오기
	HDC hdc = GetDC(m_hWnd);

	//이미지 크기

	//메인 DC와 동일한 성질의 DC 만들기
	m_ImageDC = CreateCompatibleDC(hdc);

	//이미지 불러오기 (파일경로로 직접 로딩 )
	MyImgBit = (HBITMAP)LoadImage(NULL, "Resource/Image/Title/Title.bmp",
		IMAGE_BITMAP, SCREEN_WIDTH, SCREEN_HEIGHT, LR_LOADFROMFILE);

	//이미지를 DC에 저장하기
	OldImgBit = (HBITMAP)SelectObject(m_ImageDC, MyImgBit);

	//얻은 DC 해제하기
	ReleaseDC(m_hWnd, hdc);

	//알파값을 0으로 지정해서 시작
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

	//이 3가지에 대해서는 아직 공부중..일단 이대로 세팅
	m_BlendFution.BlendOp = AC_SRC_OVER;
	m_BlendFution.BlendFlags = 0;
	m_BlendFution.AlphaFormat = 0;

	//알파값 ( 0 - 255 ) 수가 클수록 선명하다.
	m_BlendFution.SourceConstantAlpha = m_nAlpha;


	//알파블랜드 함수이다. 
	AlphaBlend(hdc,			//출력할 곳의 DC
		0,					//출력할 DC에서 그림의 찍을 좌표 x
		0,					//출력할 DC에서 그림의 찍을 좌표 y
		SCREEN_WIDTH,		//출력한 DC에서 그림의 가로 길이
		SCREEN_HEIGHT,		//출력한 DC에서 그림의 세로 길이
		m_ImageDC,			//그림의 DC
		0,					//그림에서 그리기 시작할 좌표 x
		0,					//그림에서 그리기 시작할 좌표 y
		SCREEN_WIDTH,		//그림에서 출력DC에 그릴 그림의 가로 길이
		SCREEN_HEIGHT,		//그림에서 출력DC에 그릴 그림의 세로 길이
		m_BlendFution);	//알파블렌드에서의 옵션값
}

void TitleScene::Timer(const double count) {
	//알파값을 점점 더해서 선명하게 만든다.
	
	if (m_isOnUpAlpha)
		m_nAlpha+= 3;
	else
		m_nAlpha-= 5;

	//한계수치가 넘을경우 0으로 초기화 한다.
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
