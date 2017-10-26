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
	float m_totalDistance = 0;

	BaseCharacter* m_unit;
	State m_state;
	BoundingBox m_bb;

	float m_fallSpeed = 0;
	float m_jumpSpeed = 0;

public:
	Pawn();
	Pawn(CharacterName inputCharacterName);
	~Pawn();
	Pawn(float x, float y );

	virtual void Update(State state);
	virtual void Draw(HDC hdc, State state);
	//virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY);

	virtual bool InsertKey(WPARAM wParam);

	State GetState() {
		return m_state;
	}

	void SetState(State state) {
		m_state = state;
	}

	BaseCharacter& GetUnit() {
		return *m_unit;
	}

	float GetSpeed() {
		return m_speed;
	}

	float GetBaseSpeed() {
		return m_baseSpeed;
	}

	float GetTotalDistance() {
		return m_totalDistance;
	}

	float GetCombo() {
		return m_combo;
	}

	void SetCombo(int val = 0) {
		m_combo = val;
	}

	void SetFallSpeed(int val = 0) {
		m_fallSpeed = 2.0;
	} 

	void ComputeTotalDistance();
	void ProcessCombo();
	void ProcessGravity();
	void ProcessJump();

	void ResetFallSpeed();
	void ResetJumpSpeed();
	void ResetBaseSpeed();
};

