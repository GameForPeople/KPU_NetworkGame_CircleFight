#pragma once
#include "StaticActor.h"
class Map :
	public StaticActor
{
private:
	Pos2d m_2ndImgPos;
public:
	Map();
	~Map();
	Map(float x, float y, char* imgLink);

	virtual void Draw(HDC hdc);
	virtual void Update(float Speed);
	void Update2ndImgPos( float Speed);
};

