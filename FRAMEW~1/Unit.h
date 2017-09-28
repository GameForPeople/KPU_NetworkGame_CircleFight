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
		  Jump
		, Boost
		, Item1
		, Item2
		, Item3
	};

protected:

	//������ ��ũ ���� ����
	CAnimationSprite	m_sprite;

	//POINT				m_ptCurrent;
	float				m_Speed = 0;
	float				m_CurrentJump = 0;
	int					m_isJump = 0;
	int					m_isPushShift = false;

	// ���� ���� ����(millisec)
	//float				m_fFrequency = 1.0f / 60.0f;
	float				m_fTick = 0.f;

	// ���� �߰� ����
	int					m_comboCount = 0;
	int					m_item[3]{ 0 };

public:
	//������ ��ũ ���� �Լ�
	CUnit(LPCTSTR path, int w, int h);
	virtual ~CUnit();

	virtual void Input(InputKey input, bool bPushLShift = false);
	virtual void OnDraw(HDC hDC);
	virtual void OnUpdate(float fTimeElapsed);

	virtual void Move(POINT dir);
	// �ʱⰪ�� �⺻ ���� ����
	virtual void Jump(float maxJump = 50.f);

	// ���� �߰� �Լ�

	//virtual void UseItem();
};


