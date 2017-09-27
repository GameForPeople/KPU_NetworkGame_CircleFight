#pragma once
#include "BaseObject.h"
#include "stdafx.h"

#define FULL_IMG_COUNT 7
#define RUN_IMG 7

struct BoundingBox {
	
};

class Pawn :
	public BaseObject
{
	enum class State {
			Run
		,	Stun
		,	Sleep
		,	Slow
		,	Boost
		,	Jump
		,	DoupleJump
		,	TripleJump
		,	Fall
		,	Collide
	};

	enum class Role {
			Runner
		,	Attacker
		,	Supporter
	};

private:
	CImage m_imgArr[FULL_IMG_COUNT];
	int m_combo = 0;
	
	State m_state;
	Role m_role;

	BoundingBox m_bb;

public:
	Pawn();
	~Pawn();
	Pawn(float x, float y, Role );

	virtual void Update();
	virtual void Draw();

	void InsertKey();
};

