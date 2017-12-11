// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ ��Ÿ�� ��� �����Դϴ�.
#include <iostream>
#include <chrono>

// CImage �� ���� ��� �����Դϴ�.
#include <atlimage.h>
#include <time.h>
#include <math.h>
#include <MMSystem.h>

//
#include <atomic>

#include <vector>
#include <deque>
#include <list>

//#define DEBUG_MODE // to print log message

using namespace std;

//#include"Sound.h"
#pragma comment(lib, "winmm.lib" )
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define SERVERPORT 9000
#define HOSTPORT 9001

#define ARCHER_BASE_SPEED 6.0f
#define ZOMBIE_BASE_SPEED 5.98f
#define KNIGHT_BASE_SPEED 5.96f
#define WICHER_BASE_SPEED 5.96f

#define MAX_PLAYER 4

#define PLAT_MAX_NUMBER 1015
#define PLAT_WIDTH 100
#define PLAT_HEIGHT 50
#define PLAT_LOW_HEIGHT 600

#define PLAT_MAX_NUMBER 1015
#define PLAT_WIDTH 100
#define PLAT_HEIGHT 50
#define PLAT_LOW_HEIGHT 600

#define PLAT_SHOWN_CNT 16

#define PLAT_ITEM_HEIGHT		100		//������ ����
#define ITEM_INTERVAL			50		//�����۰� ����
#define ITEM_FIRST_IMPACT		24		//������ ó�� ������ �� - 1
#define ITEM_SIZE				100		//������ �׸��� ũ��
#define LIGHTNING_DELAY			1.5f
#define LIGHTNING_DURATION		2.0F
#define LIGHTNING_DURATION_B	4.0F
#define BED_DELAY				1.0f
#define BED_DURATION			3.0F
#define BED_DURATION_B			5.0F
#define SHEILD_DELAY			0.0f
#define SHEILD_DURATION			3.0F
#define SHEILD_DURATION_B		6.0F
#define WING_DELAY				0.0f
#define WING_DURATION			3.0F
#define WING_DURATION_B			5.0F


enum class State {
		Run
	,	Stun
	,	Sleep
	,	Slow
	,	Boost
	,	JumpStart
	,	JumpLoop
	,	JumpEnd
	,	DoubleJumpStart
	,	DoubleJumpLoop
	,	DoubleJumpEnd
	,	TripleJumpStart
	,	TripleJumpLoop
	,	TripleJumpEnd
	,	TripleJump
	,	Fall
	,	Collide
	,	Death
	,	Revival
};

enum class CharacterName {
		Archer
	,	Zombie
	,	Knight
	,	Wicher
	,	NONE
};

enum class SceneName {
		Title
	,	Login
	,	Lobby
	,	Room
	,	RoomGuest
	,	InGame
	,	InGameGuest
};

enum class MapName{
		Sea
	,	Forest
};

enum item			//������ �̳�
{
	LIGHTNING = 1,		//����
	BED,			//ħ��
	WING,			//����
	SHEILD,			//����
	TIMEOUT_FAINT,	//���� ����
	TIMEOUT_SLEEP,	//�� ����
	TIMEOUT_PROTECT,//��� ����
	TIMEOUT_SPEEDUP	//�ӵ� ��� ����
};



