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
#include <deque>
#include <atomic>

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

#define ARCHER_BASE_SPEED 6.0f
#define ZOMBIE_BASE_SPEED 5.98f
#define KNIGHT_BASE_SPEED 5.96f
#define WICHER_BASE_SPEED 5.96f

#define My_Map_Is ==
#define Your_New_Map_Is =


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



//------------------------------------------- Room 통신용 -----------------------------------------------------
#define MAX_PLAYER 4

enum { ROOMCHNG = 301, CHACHNG, MAPCHNG, REQEXIT, NOTIFYEXIT, NOTIFYSTART };

struct RoomConnect {
	int idx;
	SOCKET sock;

	RoomConnect(){}
	RoomConnect(int idx, SOCKET sock) :idx(idx), sock(sock) {}
};

struct QueueData {
	int op;
	int fromIdx;

	QueueData(int op = NULL, int fromIdx = NULL)
		:op(op), fromIdx(fromIdx) {}
};

struct RoomInfo {
	CharacterName charInfo[4];
	MapName mapInfo;
};

struct ChaChng {
	int idx;
	CharacterName charInfo;

	ChaChng() {}
	ChaChng(int idx, CharacterName charInfo) : idx(idx), charInfo(charInfo) {}
};


//------------------------------------------- InGame 통신용 -----------------------------------------------------

enum { BASICINFO = 401, INPUT_JUMP, INPUT_EMOTION };

struct Pos2d {
	float x;
	float y;
};

struct FirstPlatInfo {
	int idx;
	float xPos;
};

struct BasicInfo {
	Pos2d position[MAX_PLAYER];		// 캐릭터 위치
	float totalDis[MAX_PLAYER];			// 캐릭터 속도
	float speed[MAX_PLAYER];			// 캐릭터 속도
	State state[MAX_PLAYER];			// 캐릭터 현재 상태
	int imgCnt[MAX_PLAYER];			// 애니메이션 이미지 카운트
	float combo[MAX_PLAYER];			// 캐릭터 콤보 상태
	int emoticon[MAX_PLAYER];		// 캐릭터 감정 표현
	FirstPlatInfo platInfo[MAX_PLAYER];					// 첫 발판 정보
};