#pragma once
#include "BaseCharacter.h"

#define WICHER_MAX_IMAGE_NUM 47
#define WICHER_RUN_IMAGE_NUM 20
#define WICHER_JUMP_IMAGE_NUM 27

class CWicher :
	public BaseCharacter
{
public:
	CWicher();
	~CWicher();

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
		return  WICHER_RUN_IMAGE_NUM;
	}
};

