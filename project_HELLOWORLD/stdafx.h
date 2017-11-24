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
};

enum class MapName{
		Sea
	,	Forest
};




//------------------------------------------- Room ��ſ� -----------------------------------------------------
#define MAX_PLAYER 4

enum { ROOMCHNG = 301, CHACHNG, MAPCHNG, REQEXIT, NOTIFYEXIT };

struct RoomConnect {
	int idx;
	SOCKET sock;

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
	int mapInfo;
};

struct ChaChng {
	int idx;
	int charNum;

	ChaChng(int idx, int charNum) : idx(idx), charNum(charNum) {}
};
