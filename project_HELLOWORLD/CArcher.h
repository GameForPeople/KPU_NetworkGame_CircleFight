#pragma once
#include "BaseCharacter.h"

#define ARCHER_MAX_IMAGE_NUM 51
#define ARCHER_RUN_IMAGE_NUM 21
#define ARCHER_JUMP_IMAGE_NUM 30

class CArcher :
	public BaseCharacter
{
public:
	CArcher();
	~CArcher();

	virtual void ChangeImage(State state);
	virtual void Update(State state);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state);

	virtual void LoadCharacterImage();

	virtual float GetBaseSpeed() {
		return m_baseSpeed;
	}
};

