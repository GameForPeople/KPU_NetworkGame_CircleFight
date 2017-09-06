#include "stdafx.h"
#include "Unit.h"
#include "SpriteImage.h"

CUnit::CUnit(LPCTSTR path, int w, int h, POINT pt, float fFrequency)
	: m_sprite(path, w, h)
{
	// �ʱ� ��ġ
	m_ptCurrent = pt;
	// 1ȸ ȣ�� �� ������ �ӷ�
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
	// Ű ���� ���� : idle ���·� ����
	case InputKey::KeyUp:
		m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_idle);
		break;
	case InputKey::MoveLeft:
		// �������� ���� ����
		m_sprite.SetDirection(true);
		// �������� �̵�
		return Move({ -1,0 });
	case InputKey::MoveRight:
		// ���������� ���� ����
		m_sprite.SetDirection(false);
		// ���������� �̵�
		return Move({ 1,0 });
	case InputKey::Jump:		
		// ����
		m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_jump);
		// ���� Ȱ��ȭ
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

	// ��������Ʈ ����
	m_sprite.OnUpdate();
	// ���� Ȱ��ȭ ���̸� ����
	if(m_bJump) Jump();
}

void CUnit::Move(POINT dir)
{
	// move ���·� ����
	m_sprite.UpdateState(CAnimationSprite::SpriteState::sprite_state_move);
	// ���� ������ ��쿡�� ����(�غ� �ڼ��̱� ����)
	if (m_sprite.GetCurrentStateState() == CAnimationSprite::StateState::state_state_excute) return;

	// Shift�� ���� ���ӵ� ����
	int accel = m_bPushShift ? 2.8 : 1;

	// �̵�
	m_ptCurrent.x += LONG(dir.x * m_fSpeed * accel);
	m_ptCurrent.y += LONG(dir.y * m_fSpeed * accel);

}

void CUnit::Jump(float maxJump)
{
	int curJumpCnt = m_sprite.GetCurrentAnimationCount();
	int maxJumpCnt = m_sprite.GetMaxState(CAnimationSprite::SpriteState::sprite_state_jump);
	int midJumpCnt = maxJumpCnt / 2;

	// - �� / 2 ~ �� / 2 ������ cos �� ��ŭ �̵�. �������� �ϸ��ϰ� �ö󰬴� �������� ȿ�� �ο�
	float �� = 3.141592f;
	m_fCurrentJump = maxJump * cos(float(float(midJumpCnt - curJumpCnt) / midJumpCnt) * (�� * 0.5f));

	// ���� ���°� �ƴ϶�� ���� ��Ȱ��ȭ
	if (m_sprite.GetCurrentState() != CAnimationSprite::SpriteState::sprite_state_jump) m_bJump = false;

}
