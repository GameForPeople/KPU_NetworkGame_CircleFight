#pragma once
#include "BaseObject.h"
#include "stdafx.h"
#include "BaseCharacter.h"
#include "CArcher.h"

struct BoundingBox {
	
};

class Pawn :
	public BaseObject
{
private:
	int m_combo = 0;
	float m_speed = 0;
	float m_baseSpeed = 0;

	BaseCharacter* m_unit;
	State m_state;
	BoundingBox m_bb;

	bool m_isFalling = false;
	float m_fallSpeed = 0;

	bool m_isJump = false;
	float m_jumpSpeed = 0;

public:
	Pawn();
	Pawn(CharacterName inputCharacterName);
	~Pawn();
	Pawn(float x, float y );

	virtual void Update(State state);
	virtual void Draw(HDC hdc);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY);

	void InsertKey(WPARAM wParam);

	State GetState() {
		return m_state;
	}

	BaseCharacter& GetUnit() {
		return *m_unit;
	}

	float GetSpeed() {
		return m_speed;
	}

};

