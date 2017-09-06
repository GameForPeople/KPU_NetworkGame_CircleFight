#pragma once

#include <windows.h>
#include <atlImage.h>

class CSpriteImage
{

protected:

	CImage m_ciSpriteImage;

	int m_nFrameXNum;
	int m_nFrameYNum;

	int m_nFrameWidthSize;
	int m_nFrameHeightSize;

	POINT m_ptCurrentAnimationState{ 0,0 };

	COLORREF m_colTransparent;

public:
	CSpriteImage(LPCTSTR path, int w, int h);
	virtual ~CSpriteImage();
	
	virtual void OnUpdate();

	virtual void Draw(HDC hDC, int x, int y);
};


/*
	class CAnimationSprite
	- Ⱦ��ũ��(��, ��) ���� �ִϸ��̼� ��������Ʈ Ŭ����

	- ���¸� �����Ͽ� �̹����� ���¿� ���� ó���� �� �ְ� ����
	- �̹����� Ȧ�� ��° : ������ ���� / ¦�� ��° : ���� �������� ��ġ �ʿ�
	- ���� ���� ����, ���� ���� ����, ���� ���� ���¿� ���� �� ������ ����
	- �ܼ��� ���� - ������ �������� �ʾƵ� ��� ����
	
*/
class CAnimationSprite : public CSpriteImage
{
public:
	// Sprite State : ĳ������ ���� ����
	// �ʿ信 ���� ���� �ʿ�
	typedef enum {
		  sprite_state_idle = 0
		, sprite_state_move
		, sprite_state_jump
		, sprite_state_count
	}SpriteState;

	// ������ ����(����/����/����)
	// �������� ���ÿ�
	typedef enum {
		  state_state_excute = 0
		, state_state_action
		, state_state_exit
		
	}StateState;

protected:

	// ���¿� ���� ��ü �̹��� ��
	int m_iMaxState		[SpriteState::sprite_state_count];
	// ���¿� ���� �� �̹����� �ִϸ��̼� ������
	// ���� - ���� - ���� ������ ���
	// ����-���� ���°� ������ 0
	int m_iStateDelay	[SpriteState::sprite_state_count][3];

	// ���¿� ���� ���� ������ ���۰� �� �̹��� ��ȣ
	int m_iStateExcute	[SpriteState::sprite_state_count][2];
	// ���¿� ���� ���� ������ ���۰� �� �̹��� ��ȣ
	int m_iStateAction	[SpriteState::sprite_state_count][2];
	// ���¿� ���� ���� ������ ���۰� �� �̹��� ��ȣ
	int m_iStateExit	[SpriteState::sprite_state_count][2];

	// ���� ����
	SpriteState m_stCurrent = sprite_state_idle;
	// ���� ���� : ���� ���� ���� �� �����ߴٰ� ���� ���� ���� �� ���� ���·� ����
	SpriteState m_stNext = sprite_state_idle;

	// ������ ī����
	// m_iStateDelay�� ����� ����ŭ Update�� ����
	// Update�� ���� ���� �ִϸ��̼� ���� ���� 
	// = ���� �̹����� ���� �ε巯�� �ִϸ��̼� ȿ�� ���� ����
	int m_nCount = 0;

	// ���� ����
	bool m_bLeft = false;
public:

	CAnimationSprite(LPCTSTR path, int w, int h);
	virtual ~CAnimationSprite();

	// State �� ���� : ���� ������ �̹��� ��ȣ ����. ���� - ���� ���°� ���� �� ���
	void SetState(SpriteState s, int val);
	// State �� ���� : ��� ������ �̹��� ��ȣ ����. ���� ���´� ���� - ���� ������ ���� ���� ����
	void SetState(SpriteState s, int val, int excute_start, int excute_end, int exit_start, int exit_end);
	
	// Delay �� ���� : ���� - ���� - ���� ������ Delay �� ����
	void SetStateDelay(SpriteState s, int excute, int action, int exit)
	{ m_iStateDelay[s][0] = excute; m_iStateDelay[s][1] = action; m_iStateDelay[s][2] = exit; }
	// Delay �� ���� : ���� ������ Delay �� ����. ������ ���� Delay�� 0.
	void SetStateDelay(SpriteState s, int val)
	{ m_iStateDelay[s][0] = 0; m_iStateDelay[s][1] = val; m_iStateDelay[s][2] = 0; }
	// ���ڷ� �Ѱܹ��� ���·� ����
	void UpdateState(SpriteState s);
	
	// ���� ������ �˷��ִ� Getter �Լ�
	bool IsLeft() const { return m_bLeft; }
	// ������ �����ϴ� Setter �Լ�
	void SetDirection(bool bLeft);

	// ���°� ������ ���� - ���� - ���� �̹��� ���� ���� ��ȯ
	int GetMaxState(SpriteState s) const { return m_iMaxState[s]; }
	// ���� ���¸� ��ȯ
	SpriteState GetCurrentState() const { return m_stCurrent; }
	// ���� ������� �̹��� ��ȣ�� ��ȯ
	LONG GetCurrentAnimationCount() const { return m_ptCurrentAnimationState.x; }
	// ���� ���°� � ��������(���� - ���� - ����) ��ȯ
	StateState GetCurrentStateState() const;

	// ����
	virtual void OnUpdate();

	// �׸���
	virtual void Draw(HDC hDC, int x, int y);
};

