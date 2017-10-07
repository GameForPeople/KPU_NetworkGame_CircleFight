#pragma once

#include <windows.h>

struct Pos2d {
	float x;
	float y;
};

class BaseObject
{
protected:
	Pos2d m_pos;
	Pos2d m_size;
	//float m_alphaValue;
public:
	BaseObject();
	~BaseObject();
	BaseObject(float x, float y);
	BaseObject::BaseObject(float x, float y, float sizeX, float sizeY);

	void SetPos(float x, float y);

	virtual void Update(float speed);
	virtual void Draw(HDC);

	Pos2d GetPos() {
		return m_pos;
	}
};

