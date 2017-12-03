#pragma once
#include "stdafx.h"
#include "BaseObject.h"
#include "BaseCharacter.h"
#include "CArcher.h"
#include "CZombie.h"
#include "CKnight.h"
#include "CWicher.h"


#define			INIT_PAWN_POS_X			200
#define			SPEED_BUFF_VAL			1.2f
#define			SPEED_BUFF_VAL_B		1.4f


struct BoundingBox {
	
};

class Pawn :
	public BaseObject
{
private:
	BaseCharacter* m_unit;
	State m_state;
	State m_preState;
	//BoundingBox m_bb;
	CharacterName m_charType;

	int m_combo = 0;
	int m_comboStandard = 0;
	bool m_comboEffect = true;

	float m_speed = 0;
	float m_bufSpeed = 1.0f;
	float m_baseSpeed = 0;
	float m_totalDistance = 0;
	

	float m_fallSpeed = 0;
	float m_jumpSpeed = 0;


	int m_stackFaint = 0;
	int m_stackSheild = 0;
	int m_stackSpeedUp = 0;

public:
	Pawn();
	Pawn(CharacterName inputCharacterName);
	~Pawn();
	Pawn(float x, float y );

	virtual void Update(State state, double time);
	virtual void Draw(HDC hdc, float xDiff, State state);
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

	CharacterName GetCharType() {
		return m_charType;
	}

	int GetFaint() {
		return m_stackFaint;
	}

	void ComputeTotalDistance();
	void ProcessCombo();
	void ProcessGravity();
	void ProcessJump();

	void ResetFallSpeed();
	void ResetJumpSpeed();
	void ResetBaseSpeed();
	void ResetCombo();

	//for network
	void NetworkDrawCharacter(HDC hdc, float playerDisX, float thisDisX, float thisY, int cImageIndex, State state);

	void FaintCountUp(bool init);
	void FaintCountDown();
	void FaintReset();

	void SpeedUpCountUp(bool boost);
	void SpeedUpCountDown();
};

