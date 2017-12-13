#include "CWicher.h"



CWicher::CWicher()
{
	LoadCharacterImage();

	m_imageCount = 0;
	m_imageTimer = 0;
	m_baseSpeed = WICHER_BASE_SPEED;
}


CWicher::~CWicher()
{
	for (int i = 0; i < WICHER_MAX_IMAGE_NUM; i++)
		m_imgArr[i].Destroy();
}





void CWicher::Update(State state, double time) {
	ChangeImage(state);
}

void CWicher::Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state) {

	if (m_imageCount < WICHER_RUN_IMAGE_NUM)
		m_imgArr[m_imageCount].TransparentBlt(hdc, x, y, 250, 220, RGB(255, 255, 255));
	else if (m_imageCount < WICHER_MAX_IMAGE_NUM)
		m_imgArr[m_imageCount].TransparentBlt(hdc, x, y, 270, 246, RGB(255, 255, 255));
}






void CWicher::ChangeImage(State state) {
	m_imageTimer++;

	if (state == State::Run) {
		if (m_imageTimer % 2 == 0) {
			m_imageTimer = 0;

			m_imageCount++;

			if (m_imageCount >= WICHER_RUN_IMAGE_NUM)
				m_imageCount = 0;
		}
	}
	else if (state == State::JumpLoop || state == State::JumpEnd
		|| state == State::DoubleJumpLoop || state == State::DoubleJumpEnd) {
		if (m_imageTimer % 2 == 0) {
			m_imageTimer = 0;

			m_imageCount++;

			if (m_imageCount >= 49)
				m_imageCount = 49;
			else if (m_imageCount >= WICHER_MAX_IMAGE_NUM)
				m_imageCount = WICHER_MAX_IMAGE_NUM - 1;
		}
	}
	else if (state == State::Fall) {
		m_imageCount = 0;
	}
	else {
		if (m_imageCount >= WICHER_RUN_IMAGE_NUM)
			m_imageCount = 1;
	}
}

void CWicher::LoadCharacterImage() {

	m_imgArr = new CImage[WICHER_MAX_IMAGE_NUM];

	m_imgArr[0].Load("Resource/Image/Wicher/Wicher_Run_Edit_0.png");
	m_imgArr[1].Load("Resource/Image/Wicher/Wicher_Run_Edit_1.png");
	m_imgArr[2].Load("Resource/Image/Wicher/Wicher_Run_Edit_2.png");
	m_imgArr[3].Load("Resource/Image/Wicher/Wicher_Run_Edit_3.png");
	m_imgArr[4].Load("Resource/Image/Wicher/Wicher_Run_Edit_4.png");
	m_imgArr[5].Load("Resource/Image/Wicher/Wicher_Run_Edit_5.png");
	m_imgArr[6].Load("Resource/Image/Wicher/Wicher_Run_Edit_6.png");
	m_imgArr[7].Load("Resource/Image/Wicher/Wicher_Run_Edit_7.png");
	m_imgArr[8].Load("Resource/Image/Wicher/Wicher_Run_Edit_8.png");
	m_imgArr[9].Load("Resource/Image/Wicher/Wicher_Run_Edit_9.png");
	m_imgArr[10].Load("Resource/Image/Wicher/Wicher_Run_Edit_10.png");
	m_imgArr[11].Load("Resource/Image/Wicher/Wicher_Run_Edit_11.png");
	m_imgArr[12].Load("Resource/Image/Wicher/Wicher_Run_Edit_12.png");
	m_imgArr[13].Load("Resource/Image/Wicher/Wicher_Run_Edit_13.png");
	m_imgArr[14].Load("Resource/Image/Wicher/Wicher_Run_Edit_14.png");
	m_imgArr[15].Load("Resource/Image/Wicher/Wicher_Run_Edit_15.png");
	m_imgArr[16].Load("Resource/Image/Wicher/Wicher_Run_Edit_16.png");
	m_imgArr[17].Load("Resource/Image/Wicher/Wicher_Run_Edit_17.png");
	m_imgArr[18].Load("Resource/Image/Wicher/Wicher_Run_Edit_18.png");
	m_imgArr[19].Load("Resource/Image/Wicher/Wicher_Run_Edit_19.png");
	
	m_imgArr[20].Load("Resource/Image/Wicher/Wicher_Jump_Edit_0.png");

	m_imgArr[21].Load("Resource/Image/Wicher/Wicher_Jump_Edit_1.png");
	m_imgArr[22].Load("Resource/Image/Wicher/Wicher_Jump_Edit_2.png");
	m_imgArr[23].Load("Resource/Image/Wicher/Wicher_Jump_Edit_3.png");
	m_imgArr[24].Load("Resource/Image/Wicher/Wicher_Jump_Edit_4.png");
	m_imgArr[25].Load("Resource/Image/Wicher/Wicher_Jump_Edit_5.png");
	m_imgArr[26].Load("Resource/Image/Wicher/Wicher_Jump_Edit_6.png");
	m_imgArr[27].Load("Resource/Image/Wicher/Wicher_Jump_Edit_7.png");
	m_imgArr[28].Load("Resource/Image/Wicher/Wicher_Jump_Edit_8.png");
	m_imgArr[29].Load("Resource/Image/Wicher/Wicher_Jump_Edit_9.png");
	m_imgArr[30].Load("Resource/Image/Wicher/Wicher_Jump_Edit_10.png");
	m_imgArr[31].Load("Resource/Image/Wicher/Wicher_Jump_Edit_11.png");
	m_imgArr[32].Load("Resource/Image/Wicher/Wicher_Jump_Edit_12.png");
	m_imgArr[33].Load("Resource/Image/Wicher/Wicher_Jump_Edit_13.png");
	m_imgArr[34].Load("Resource/Image/Wicher/Wicher_Jump_Edit_14.png");
	m_imgArr[35].Load("Resource/Image/Wicher/Wicher_Jump_Edit_15.png");
	m_imgArr[36].Load("Resource/Image/Wicher/Wicher_Jump_Edit_16.png");
	m_imgArr[37].Load("Resource/Image/Wicher/Wicher_Jump_Edit_17.png");
	m_imgArr[38].Load("Resource/Image/Wicher/Wicher_Jump_Edit_18.png");
	m_imgArr[39].Load("Resource/Image/Wicher/Wicher_Jump_Edit_19.png");
	m_imgArr[40].Load("Resource/Image/Wicher/Wicher_Jump_Edit_20.png");
	m_imgArr[41].Load("Resource/Image/Wicher/Wicher_Jump_Edit_21.png");
	m_imgArr[42].Load("Resource/Image/Wicher/Wicher_Jump_Edit_22.png");
	m_imgArr[43].Load("Resource/Image/Wicher/Wicher_Jump_Edit_23.png");
	m_imgArr[44].Load("Resource/Image/Wicher/Wicher_Jump_Edit_24.png");
	m_imgArr[45].Load("Resource/Image/Wicher/Wicher_Jump_Edit_25.png");
	m_imgArr[46].Load("Resource/Image/Wicher/Wicher_Jump_Edit_26.png");
}