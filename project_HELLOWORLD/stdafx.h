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

using namespace std;

//#include"Sound.h"
#pragma comment(lib, "winmm.lib" )
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

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
	,	TripleJump
	,	Fall
	,	Collide
	,	Death
	,	Revival
};

enum class CharacterName {
	Archer
};
