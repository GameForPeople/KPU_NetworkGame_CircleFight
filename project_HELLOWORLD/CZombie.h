#pragma once
#include "BaseCharacter.h"

#define ZOMBIE_MAX_IMAGE_NUM 59
#define ZOMBIE_RUN_IMAGE_NUM 32
#define ZOMBIE_JUMP_IMAGE_NUM 27

class CZombie :
	public BaseCharacter
{
public:
	CZombie();
	~CZombie();

	virtual void Update(State state);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state);
	
public:
	virtual void ChangeImage(State state);
	virtual void LoadCharacterImage();


public:
	virtual float GetBaseSpeed() const {
		return m_baseSpeed;
	}
	virtual int GetJumpImageCount() const {
		return ZOMBIE_RUN_IMAGE_NUM;
	}

};

