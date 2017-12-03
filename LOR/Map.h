#pragma once
#include "StaticActor.h"

//why StaticActor?

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
	virtual void Update(const float Speed, const double time);

public:
	void Update2ndImgPos(const float Speed, const double time);
};

