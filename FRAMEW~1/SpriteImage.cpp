#include "stdafx.h"
#include "SpriteImage.h"
#include <iostream>
#include <cassert>

using namespace std;

CSpriteImage::CSpriteImage(LPCTSTR path, int w, int h)
{

	m_nFrameXNum = w;
	m_nFrameYNum = h;

	// ���� �߻��ϸ� �� �Ǵ� �ڵ��̱� ������ �߻� �� ������ ���α׷��� ����
	if (FAILED(m_ciSpriteImage.Load(path)))
		assert(!"�̹����� ���������� �ε���� �ʾҽ��ϴ�!");


	m_nFrameWidthSize = m_ciSpriteImage.GetWidth() / m_nFrameXNum;
	m_nFrameHeightSize = m_ciSpriteImage.GetHeight() / m_nFrameYNum;

	m_colTransparent = ::GetPixel(m_ciSpriteImage.GetDC(), 0, 0);
	
}

CSpriteImage::~CSpriteImage()
{
	if (!m_ciSpriteImage.IsNull())
		m_ciSpriteImage.Destroy();
}

void CSpriteImage::OnUpdate()
{
	m_ptCurrentAnimationState.x++;

	if (m_ptCurrentAnimationState.x >= m_nFrameXNum)
	{
		m_ptCurrentAnimationState.x = 0;
		m_ptCurrentAnimationState.y++;
	}

	if (m_ptCurrentAnimationState.y >= m_nFrameYNum)
	{
		m_ptCurrentAnimationState.y = 0;
	}
}

void CSpriteImage::Draw(HDC hDC, int x, int y)
{
	m_ciSpriteImage.TransparentBlt(hDC,
		x, y,
		m_nFrameWidthSize, m_nFrameHeightSize,
		m_ptCurrentAnimationState.x * m_nFrameWidthSize,
		m_ptCurrentAnimationState.y * m_nFrameHeightSize,
		m_nFrameWidthSize, m_nFrameHeightSize,
		m_colTransparent
	);

}



CAnimationSprite::CAnimationSprite(LPCTSTR path, int w, int h) : CSpriteImage(path, w, h)
{

}

CAnimationSprite::~CAnimationSprite()
{
}

void CAnimationSprite::SetState(SpriteState s, int val)
{
	m_iMaxState[s] = val;

	m_iStateExcute[s][0] = 0;
	m_iStateExcute[s][1] = 0;
	m_iStateExit[s][0] = val;
	m_iStateExit[s][1] = val;
	m_iStateAction[s][0] = 0;
	m_iStateAction[s][1] = val;
}

void CAnimationSprite::SetState(SpriteState s, int val, int excute_start, int excute_end, int exit_start, int exit_end)
{
	m_iMaxState[s] = val;

	m_iStateExcute[s][0] = excute_start;
	m_iStateExcute[s][1] = excute_end;
	m_iStateExit[s][0] = exit_start;
	m_iStateExit[s][1] = exit_end;
	m_iStateAction[s][0] = excute_end + 1;
	m_iStateAction[s][1] = exit_start;
}

void CAnimationSprite::SetDirection(bool bLeft)
{
	// ������ ����ƴ��� Ȯ��
	bool VaildChange = (m_bLeft != bLeft);
	m_bLeft = bLeft;

	// ���� �߿��� ���� ����
	if (SpriteState::sprite_state_jump == m_stCurrent) return;
	// ������� ���� ��� ��ó�� ����
	if (!VaildChange) return;

	// ������ ī���� �ʱ�ȭ
	m_nCount = 0;
	// ���� ���·� ����
	m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
	m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
}

void CAnimationSprite::UpdateState(SpriteState s)
{
	// ���� �߿��� ���� ���� ����
	if (SpriteState::sprite_state_jump == m_stCurrent) return;
	// ���� ���¿� �����ϸ� ����
	if (s == m_stCurrent) return;

	// ���� ���°� idle �� �ƴϰ�
	// ���� �������� ���°� ���� ������ ���
	// ���� ���·� �������� �ʰ� ����
	if (SpriteState::sprite_state_idle != s 
		|| GetCurrentStateState() == StateState::state_state_excute)
	{
		m_stCurrent = s;
		m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
		m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
	}
	// ���� ���·� ����
	else
	{
		// �̹� �������̸� ���
		if (GetCurrentStateState() == StateState::state_state_exit) return;
		m_stNext = s;
		m_ptCurrentAnimationState.x = m_iStateExit[m_stCurrent][0];
	}

	// ������ ī���� �ʱ�ȭ
	m_nCount = 0;

}

CAnimationSprite::StateState CAnimationSprite::GetCurrentStateState() const
{
	// ���� ���·� �ʱ�ȭ
	StateState State_state = state_state_excute;
	// ���� ���º��� ���� �����ϰ� �ִ� ���
	if (m_ptCurrentAnimationState.x >= m_iStateAction[m_stCurrent][1]) State_state = state_state_exit;
	// ��������� ���
	else if (m_ptCurrentAnimationState.x >= m_iStateAction[m_stCurrent][0]) State_state = state_state_action;
	return State_state;
}

void CAnimationSprite::OnUpdate()
{
	// excute - action - exit
	StateState State_state = GetCurrentStateState();

	// ������ ī���� ����
	(++m_nCount) %= (m_iStateDelay[m_stCurrent][State_state]);
	// ������ ī���ͷ� ���� ���� Ȯ��
	if(m_nCount) return;

	// ���� ���°� �ƴ� ���
	if (State_state < state_state_exit)
	{
		// �̹����� �����ϵ�
		(++m_ptCurrentAnimationState.x) %= m_iStateAction[m_stCurrent][1];
		// �ʱ�ȭ�� ��������� ó������
		if (0 == m_ptCurrentAnimationState.x) m_ptCurrentAnimationState.x = m_iStateAction[m_stCurrent][0];
	}
	else
	{
		// �̹��� ����
		(++m_ptCurrentAnimationState.x) %= m_iStateExit[m_stCurrent][1];
		// �ʱ�ȭ��
		if (0 == m_ptCurrentAnimationState.x)
		{
			// ���� ���·� ����
			m_stCurrent = m_stNext;
			m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
			m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
		}
	}

	// ���� ���̰�
	if (m_stCurrent != SpriteState::sprite_state_jump)	return;
	// �̹����� �ʱ�ȭ ���¶��
	if (m_ptCurrentAnimationState.x)					return;

	// idle ���·� �ʱ�ȭ
	m_stCurrent = SpriteState::sprite_state_idle;
	m_ptCurrentAnimationState.y = (SpriteState::sprite_state_idle * 2) + (m_bLeft ? 1 : 0);
}

void CAnimationSprite::Draw(HDC hDC, int x, int y)
{
	m_ciSpriteImage.TransparentBlt(hDC,
		// Draw Start pt
		x, y,
		// Draw Size
		m_nFrameWidthSize, m_nFrameHeightSize,
			// Image Start pt.x
			m_ptCurrentAnimationState.x * m_nFrameWidthSize,
			// Image Start pt.y
			m_ptCurrentAnimationState.y * m_nFrameHeightSize,
		// Image Size
		m_nFrameWidthSize, m_nFrameHeightSize,
		// Transparent RGB
		m_colTransparent
	);

}
