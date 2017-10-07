#pragma once

#include "stdafx.h"

class BaseCharacter
{
	enum class Type {
			Runner
		,	Attacker
		,	Supporter
	};

protected:
	CImage* m_imgArr;
	int m_imageCount = 0;
	int m_imageTimer = 0;

	float m_baseSpeed;
public:
	BaseCharacter();
	~BaseCharacter();

	virtual void Update(State state);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state);

	virtual float GetBaseSpeed() {
		return m_baseSpeed;
	}

	void SetImageCount(int input) {
		m_imageCount = input;
	}

	virtual void LoadCharacterImage() = 0;
};

