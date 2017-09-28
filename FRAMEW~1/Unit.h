#pragma once

#include <Windows.h>
#include "SpriteImage.h"
/*
	class CUnit

	- 입력 가능한 경우을 내부적으로 선언하고 해당 입력을 받아 내부적으로 처리

	- 캐릭터 스프라이트에 필요한 정보는 최종 사용자가 입력해야 하므로 외부로 노출
	- 점프 가능
	- Shift를 눌러 가속 가능

	※ 각 상태에 대한 SetState와 SetStateDelay 가 입력되지 않으면 오류 발생
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

	//프레임 워크 제공 변수
	CAnimationSprite	m_sprite;

	//POINT				m_ptCurrent;
	float				m_Speed = 0;
	float				m_CurrentJump = 0;
	int					m_isJump = 0;
	int					m_isPushShift = false;

	// 유닛 갱신 단위(millisec)
	//float				m_fFrequency = 1.0f / 60.0f;
	float				m_fTick = 0.f;

	// 게임 추가 변수
	int					m_comboCount = 0;
	int					m_item[3]{ 0 };

public:
	//프레임 워크 제공 함수
	CUnit(LPCTSTR path, int w, int h);
	virtual ~CUnit();

	virtual void Input(InputKey input, bool bPushLShift = false);
	virtual void OnDraw(HDC hDC);
	virtual void OnUpdate(float fTimeElapsed);

	virtual void Move(POINT dir);
	// 초기값은 기본 점프 높이
	virtual void Jump(float maxJump = 50.f);

	// 게임 추가 함수

	//virtual void UseItem();
};


