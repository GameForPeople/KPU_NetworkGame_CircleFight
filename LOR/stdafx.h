// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

// C++ 런타임 헤더 파일입니다.
#include <iostream>
#include <chrono>

// CImage 를 위한 헤더 파일입니다.
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

#define PLAT_ITEM_HEIGHT		100		//아이템 높이
#define ITEM_INTERVAL			50		//아이템간 간격
#define ITEM_FIRST_IMPACT		24		//아이템 처음 나오는 곳 - 1
#define ITEM_SIZE				100		//아이템 그릴때 크기
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

enum item			//아이템 이넘
{
	LIGHTNING = 1,		//번개
	BED,			//침대
	WING,			//날개
	SHEILD,			//방패
	TIMEOUT_FAINT,	//번개 해제
	TIMEOUT_SLEEP,	//잠 해제
	TIMEOUT_PROTECT,//방어 해제
	TIMEOUT_SPEEDUP	//속도 상승 해제
};



