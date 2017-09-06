#include "stdafx.h"
#include "SpriteImage.h"
#include <iostream>
#include <cassert>

using namespace std;

CSpriteImage::CSpriteImage(LPCTSTR path, int w, int h)
{

	m_nFrameXNum = w;
	m_nFrameYNum = h;

	// 절대 발생하면 안 되는 코드이기 때문에 발생 시 강제로 프로그램을 종료
	if (FAILED(m_ciSpriteImage.Load(path)))
		assert(!"이미지가 정상적으로 로드되지 않았습니다!");


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
	// 방향이 변경됐는지 확인
	bool VaildChange = (m_bLeft != bLeft);
	m_bLeft = bLeft;

	// 점프 중에는 변경 생략
	if (SpriteState::sprite_state_jump == m_stCurrent) return;
	// 변경되지 않은 경우 후처리 생략
	if (!VaildChange) return;

	// 딜레이 카운터 초기화
	m_nCount = 0;
	// 진입 상태로 변경
	m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
	m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
}

void CAnimationSprite::UpdateState(SpriteState s)
{
	// 점프 중에는 상태 갱신 생략
	if (SpriteState::sprite_state_jump == m_stCurrent) return;
	// 현재 상태와 동일하면 생략
	if (s == m_stCurrent) return;

	// 현재 상태가 idle 이 아니고
	// 현재 진행중인 상태가 진입 상태일 경우
	// 퇴장 상태로 변경하지 않고 갱신
	if (SpriteState::sprite_state_idle != s 
		|| GetCurrentStateState() == StateState::state_state_excute)
	{
		m_stCurrent = s;
		m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
		m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
	}
	// 퇴장 상태로 갱신
	else
	{
		// 이미 퇴장중이면 취소
		if (GetCurrentStateState() == StateState::state_state_exit) return;
		m_stNext = s;
		m_ptCurrentAnimationState.x = m_iStateExit[m_stCurrent][0];
	}

	// 딜레이 카운터 초기화
	m_nCount = 0;

}

CAnimationSprite::StateState CAnimationSprite::GetCurrentStateState() const
{
	// 진입 상태로 초기화
	StateState State_state = state_state_excute;
	// 진행 상태보다 많이 진행하고 있는 경우
	if (m_ptCurrentAnimationState.x >= m_iStateAction[m_stCurrent][1]) State_state = state_state_exit;
	// 진행상태일 경우
	else if (m_ptCurrentAnimationState.x >= m_iStateAction[m_stCurrent][0]) State_state = state_state_action;
	return State_state;
}

void CAnimationSprite::OnUpdate()
{
	// excute - action - exit
	StateState State_state = GetCurrentStateState();

	// 딜레이 카운터 갱신
	(++m_nCount) %= (m_iStateDelay[m_stCurrent][State_state]);
	// 딜레이 카운터로 갱신 여부 확인
	if(m_nCount) return;

	// 퇴장 상태가 아닐 경우
	if (State_state < state_state_exit)
	{
		// 이미지를 갱신하되
		(++m_ptCurrentAnimationState.x) %= m_iStateAction[m_stCurrent][1];
		// 초기화는 진행상태의 처음으로
		if (0 == m_ptCurrentAnimationState.x) m_ptCurrentAnimationState.x = m_iStateAction[m_stCurrent][0];
	}
	else
	{
		// 이미지 갱신
		(++m_ptCurrentAnimationState.x) %= m_iStateExit[m_stCurrent][1];
		// 초기화는
		if (0 == m_ptCurrentAnimationState.x)
		{
			// 다음 상태로 변경
			m_stCurrent = m_stNext;
			m_ptCurrentAnimationState.x = m_iStateExcute[m_stCurrent][0];
			m_ptCurrentAnimationState.y = m_stCurrent * 2 + (m_bLeft ? 1 : 0);
		}
	}

	// 점프 중이고
	if (m_stCurrent != SpriteState::sprite_state_jump)	return;
	// 이미지가 초기화 상태라면
	if (m_ptCurrentAnimationState.x)					return;

	// idle 상태로 초기화
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
