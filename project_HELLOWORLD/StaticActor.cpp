#include "StaticActor.h"

StaticActor::StaticActor() {

}

StaticActor::StaticActor(float x, float y, char * imgLink) : BaseObject(x, y)
{
	m_staticImg = new CImage;
	m_staticImg->Load(imgLink);
}


StaticActor::~StaticActor()
{
	m_staticImg->Destroy();
}

void StaticActor::Draw(HDC hdc, bool isFullScreen) {
	
	if(!isFullScreen)
		m_staticImg->TransparentBlt(hdc, m_pos.x, m_pos.y, m_staticImg->GetWidth(), m_staticImg->GetHeight(), RGB(255, 255, 255));
	else
		m_staticImg->TransparentBlt(hdc, m_pos.x, m_pos.y, m_staticImg->GetWidth() - 15, m_staticImg->GetHeight() - 40, RGB(255, 255, 255));
}

void StaticActor::Draw(HDC hdc, bool isFullScreen, bool isDraw) {
	if (isDraw) {
		if (!isFullScreen)
			m_staticImg->TransparentBlt(hdc, m_pos.x, m_pos.y, m_staticImg->GetWidth(), m_staticImg->GetHeight(), RGB(255, 255, 255));
		else
			m_staticImg->TransparentBlt(hdc, m_pos.x, m_pos.y, m_staticImg->GetWidth() - 15, m_staticImg->GetHeight() - 40, RGB(255, 255, 255));
	}
}