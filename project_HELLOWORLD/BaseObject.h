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

public:
	BaseObject();
	~BaseObject();
	BaseObject(const float x, const float y);
	BaseObject::BaseObject(const float x, const float y, const float sizeX, const float sizeY);

	virtual void Update(const float speed, const double time);
	virtual void Draw(HDC hdc, float xDiff);
	virtual void Draw(HDC);

	void SetPos(const float x, const float y);
	void SetPos(const Pos2d pos) { m_pos = pos; }
	void SetXPos(const float x) { m_pos.x = x; }

	Pos2d GetPos() const;
	float GetYPos() { return m_pos.y; }
};