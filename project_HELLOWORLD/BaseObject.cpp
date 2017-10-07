#include "BaseObject.h"

BaseObject::BaseObject()
{
	m_pos = { 0, 0 };
	m_size = { 0, 0 };
}

BaseObject::BaseObject(float x, float y)
{
	m_pos = { x, y };
	m_size = { 0, 0 };
}

BaseObject::BaseObject(float x, float y, float sizeX, float sizeY)
{
	m_pos = { x, y };
	m_size = { sizeX, sizeY };
}


BaseObject::~BaseObject()
{
}

void BaseObject::SetPos(float x, float y) {
	m_pos.x = x;
	m_pos.y = y;
}


void BaseObject::Update(float speed) {
	m_pos.x -= speed;
}

void BaseObject::Draw(HDC hdc) {
	
}

