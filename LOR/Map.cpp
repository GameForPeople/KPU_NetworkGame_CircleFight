#include "Map.h"

Map::Map()
{
}

Map::~Map()
{
}

Map::Map(float x, float y, char* imgLink) : StaticActor(x, y, imgLink) {
	m_2ndImgPos.x = m_staticImg->GetWidth();
}




void Map::Draw(HDC hdc) {
	m_staticImg->TransparentBlt(hdc, m_pos.x, m_pos.y, m_staticImg->GetWidth(), m_staticImg->GetHeight() - 40, RGB(255, 255, 255));
	//1280 / 2�� �� ũ�Ⱑ 1 + 1/2�̱� ����!! �̰� �����ѹ����� �ٲܶ� ���������� üũ�ϱ�!!
	m_staticImg->TransparentBlt(hdc, m_2ndImgPos.x, m_pos.y, m_staticImg->GetWidth(), m_staticImg->GetHeight() - 40, RGB(255, 255, 255));
}

void Map::Update(float speed, double time) {
		m_pos.x = m_pos.x - (speed * 2 * time) ;

		Update2ndImgPos(speed, time);
}



void Map::Update2ndImgPos(float speed, double time) {
	m_2ndImgPos.x = m_2ndImgPos.x - (speed * 2 * time );

	if (m_pos.x <= -m_staticImg->GetWidth())
		m_pos.x = m_staticImg->GetWidth();

	if (m_2ndImgPos.x <= -m_staticImg->GetWidth())
		m_2ndImgPos.x = m_staticImg->GetWidth();
}
