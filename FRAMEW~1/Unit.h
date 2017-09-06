#pragma once

#include <Windows.h>
#include "SpriteImage.h"
/*
	class CUnit

	- �Է� ������ ����� ���������� �����ϰ� �ش� �Է��� �޾� ���������� ó��

	- ĳ���� ��������Ʈ�� �ʿ��� ������ ���� ����ڰ� �Է��ؾ� �ϹǷ� �ܺη� ����
	- ���� ����
	- Shift�� ���� ���� ����

	�� �� ���¿� ���� SetState�� SetStateDelay �� �Էµ��� ������ ���� �߻�
*/
class CUnit
{
public:

	enum class InputKey : short
	{
		  KeyUp
		, MoveLeft
		, MoveRight
		, Jump
	};

protected:

	CAnimationSprite	m_sprite;

	POINT				m_ptCurrent;
	float				m_fSpeed = 0;
	float				m_fCurrentJump = 0;
	bool				m_bJump = false;
	bool				m_bPushShift;

	// ���� ���� ����(millisec)
	float				m_fFrequency = 1.0f / 60.0f;
	float				m_fTick = 0.f;
public:

	CUnit(LPCTSTR path, int w, int h, POINT pt, float fFrequency = 0.1f / 60.0f);
	virtual ~CUnit();

	virtual void Input(InputKey input, bool bPushLShift = false);
	virtual void OnDraw(HDC hDC);
	virtual void OnUpdate(float fTimeElapsed);

	void SetState(CAnimationSprite::SpriteState state, int val) { m_sprite.SetState(state, val); }
	void SetState(CAnimationSprite::SpriteState state, int val, int excute_start, int excute_end, int exit_start, int exit_end) { m_sprite.SetState(state, val, excute_start, excute_end, exit_start, exit_end); }
	void SetStateDelay(CAnimationSprite::SpriteState state, int excute, int action, int exit) { m_sprite.SetStateDelay(state, excute, action, exit); }
	void SetStateDelay(CAnimationSprite::SpriteState state, int action) { m_sprite.SetStateDelay(state, 0, action, 0); }

protected:

	virtual void Move(POINT dir);
	// �ʱⰪ�� �⺻ ���� ����
	virtual void Jump(float maxJump = 50.f);
};


