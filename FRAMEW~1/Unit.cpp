#include "stdafx.h"
#include "Unit.h"
#include "SpriteImage.h"

CUnit::CUnit(LPCTSTR path, int w, int h, POINT pt, float fFrequency)
	: m_sprite(path, w, h)
{
	// 초기 위치
	m_ptCurrent = pt;
	// 1회 호출 당 나가는 속력
	m_fSpeed = 4.f;

	fFrequency = m_fFrequency;
	m_fTick = 0.f;
}


CUnit::~CUnit()
{
}

void CUnit::Input(InputKey input, bool bPushLShift)
{
	m_bPushShift = bPushLShift;
	switch (input)
	{
	// 키 놓음 상태 : idle 상태로 변경
	case InputKey::KeyUp:
		m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_idle);
		break;
	case InputKey::MoveLeft:
		// 왼쪽으로 방향 변경
		m_sprite.SetDirection(true);
		// 왼쪽으로 이동
		return Move({ -1,0 });
	case InputKey::MoveRight:
		// 오른쪽으로 방향 변경
		m_sprite.SetDirection(false);
		// 오른쪽으로 이동
		return Move({ 1,0 });
	case InputKey::Jump:		
		// 점프
		m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_jump);
		// 점프 활성화
		m_bJump = true;
		break;
	}
}

void CUnit::OnDraw(HDC hDC)
{
	m_sprite.Draw(hDC, m_ptCurrent.x, int(m_ptCurrent.y - m_fCurrentJump));
}

void CUnit::OnUpdate(float fTimeElapsed)
{
	m_fTick += fTimeElapsed;
	if (m_fTick < m_fFrequency) return;
	m_fTick -= m_fFrequency;

	// 스프라이트 갱신
	m_sprite.OnUpdate();
	// 점프 활성화 중이면 점프
	if(m_bJump) Jump();
}

void CUnit::Move(POINT dir)
{
	// move 상태로 변경
	m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_move);
	// 진입 상태일 경우에는 정지(준비 자세이기 때문)
	if (m_sprite.GetCurrentStateState() == CAnimationSprite::StateState::state_state_excute) return;

	// Shift에 따른 가속도 설정
	int accel = m_bPushShift ? 2.8 : 1;

	// 이동
	m_ptCurrent.x += LONG(dir.x * m_fSpeed * accel);
	m_ptCurrent.y += LONG(dir.y * m_fSpeed * accel);

}

void CUnit::Jump(float maxJump)
{
	int curJumpCnt = m_sprite.GetCurrentAnimationCount();
	int maxJumpCnt = m_sprite.GetMaxState(CAnimationSprite::SpriteState::sprite_state_jump);
	int midJumpCnt = maxJumpCnt / 2;

	// - π / 2 ~ π / 2 범위의 cos 값 만큼 이동. 높을수록 완만하게 올라갔다 내려오는 효과 부여
	float π = 3.141592f;
	m_fCurrentJump = maxJump * cos(float(float(midJumpCnt - curJumpCnt) / midJumpCnt) * (π * 0.5f));

	// 점프 상태가 아니라면 점프 비활성화
	if (m_sprite.GetCurrentState() != CAnimationSprite::SpriteState::sprite_state_jump) m_bJump = false;

}
