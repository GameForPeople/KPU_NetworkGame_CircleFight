#pragma once
#include "BaseCharacter.h"

#define KNIGHT_MAX_IMAGE_NUM 57
#define KNIGHT_RUN_IMAGE_NUM 21
#define KNIGHT_JUMP_IMAGE_NUM 36

class CKnight :
	public BaseCharacter
{
public:
	CKnight();
	~CKnight();

	virtual void Update(State state, double time);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state);

public:
	virtual void ChangeImage(State state);
	virtual void LoadCharacterImage();

public:
	virtual float GetBaseSpeed() const {
		return m_baseSpeed;
	}
	virtual int GetJumpImageCount() const {
		return KNIGHT_RUN_IMAGE_NUM;
	}
};

