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

	virtual void Update(State state, double time);
	virtual void Draw(HDC hdc, float x, float y, float sizeX, float sizeY, State state);


public:
	virtual float GetBaseSpeed() const;
	virtual int GetJumpImageCount() const;
	void SetImageCount(const int input);

	virtual void LoadCharacterImage() = 0;
};

