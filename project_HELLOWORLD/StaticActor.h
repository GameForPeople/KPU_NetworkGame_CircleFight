#pragma once
#include "BaseObject.h"
#include <Windows.h>
#include <atlimage.h>

class StaticActor :
	public BaseObject
{
private:
	//pos, alpha
	CImage* m_staticImg;
public:
	StaticActor();
	~StaticActor();

	StaticActor(float x, float y, char* imgLink);

	virtual void Draw(HDC hdc, bool isFullScreen);
	virtual void Draw(HDC hdc, bool isFullScreen, bool isDraw);

};

