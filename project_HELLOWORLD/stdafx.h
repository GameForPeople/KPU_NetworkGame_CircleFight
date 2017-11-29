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
#include <deque>
#include <atomic>

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
	,	InGameGuest
};

enum class MapName{
		Sea
	,	Forest
};



//------------------------------------------- Room ��ſ� -----------------------------------------------------
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


//------------------------------------------- InGame ��ſ� -----------------------------------------------------

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
	Pos2d position[MAX_PLAYER];		// ĳ���� ��ġ
	float totalDis[MAX_PLAYER];			// ĳ���� �ӵ�
	float speed[MAX_PLAYER];			// ĳ���� �ӵ�
	State state[MAX_PLAYER];			// ĳ���� ���� ����
	int imgCnt[MAX_PLAYER];			// �ִϸ��̼� �̹��� ī��Ʈ
	float combo[MAX_PLAYER];			// ĳ���� �޺� ����
	int emoticon[MAX_PLAYER];		// ĳ���� ���� ǥ��
	FirstPlatInfo platInfo[MAX_PLAYER];					// ù ���� ����
};