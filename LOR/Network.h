#pragma once

#pragma comment(lib, "ws2_32")

#pragma comment(lib, "winmm.lib" )
#include "fmod.hpp"
#pragma	comment (lib, "fmodex_vc.lib")
using namespace FMOD;

#include "stdafx.h"
#include "Pawn.h"
#include <WS2tcpip.h>

#define CHAT_BUF_SIZE		30
#define CHAT_MAX_LINE		5
#define MAX_ROOM_COUNT		8

#define ACCEPT_DENIED 18446744073709551615

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);

#pragma region [ GAME COMMUNICATION PROTOCOL]

enum Protocol {
	//LoginScene
	DEMAND_LOGIN = 100
	, FAIL_LOGIN = 101
	, PERMIT_LOGIN = 102

	//LobbyScene
	, DEMAND_CREATEROOM = 200
	, FAIL_CREATEROOM = 201
	, PERMIT_CREATEROOM = 202

	, DEMAND_JOINROOM = 203
	, FAIL_JOINROOM = 204
	, PERMIT_JOINROOM = 205

	, DEMAND_LOBBYINFO = 206
	, PERMIT_LOBBYINFO = 207

	, DEMAND_CHAT = 208
	, PERMIT_CHAT = 209

	, REQUEST_ROOMINFO = 210 //Server -> Host  
	, SUPPLY_ROOMINFO = 211	//Host	 -> Server


	, UPDATE_LOBBY = 212		// Timer -> LobbyUpdate

								//RoomScene
	, UPDATE_ROOM = 301

	, REQUEST_CHANGECHAR = 302
	, PERMIT_CHANGECHAR = 303

	, NOTIFY_CHANGEMAP = 304

	, REQUEST_EXIT = 305
	, PERMIT_EXIT = 306

	, NOTIFY_EXIT = 307
	, NOTIFY_START = 308

	, SUPPLY_GAMESTART = 395 //HOST	-> Server

	, DEMAND_EXITROOM = 396 //Host -> Server
	, PERMIT_EXITROOM = 397 //Server -> Host

	, DEMAND_RELOGIN = 398	//GUEST	-> Server	
	, PERMIT_RELOGIN = 399 //


						   //INGAMESCENE
	, UPDATE_FRAME = 401
	, INPUT_JUMP = 402
	, INPUT_EMOTION = 403
	, CHANGE_EMOTION = 404
	, NOTIFY_ITEM_THUNDER = 405
	, NOTIFY_ITEM_BED = 406
	, NOTIFY_ITEM_SHIELD = 407
	, NOTIFY_ITEM_WING = 408
	, INPUT_KEY_Q = 409
	, INPUT_KEY_W = 410
	
	, SUPPLY_GAMEEND = 499	//HOST	-> Server
};

#pragma region [ LoginScene 100 ~ 199 ]

//type 100 Client -> Server
struct DemandLoginStruct {
	int type{};	// 1일때는 로그인, 2일때는 회원가입
	int PW{};
	char ID[5];
};

// type 101 Server -> Client 로그인 실패, 회원가입 실패
struct FailLoginStruct {
};

// type 102 Server -> Client 로그인 성공, Lobby정보, 계정정보 전달
struct PermitLoginStruct {
	int winCount{};
	int loseCount{};
	// LobbyInfoData
};
#pragma endregion

#pragma region [ LobbyScene	200 ~ 299]

//Demand 클라 -> 서버
//Fail, Permit 서버 -> 클라

// type 200
struct DemandCreateRoomStruct {
};

// type 201
struct FailCreateRoomStruct {
};

// type 202
struct PermitCreateRoomStruct {
	int roomIndex;
};


// type 203
struct DemandJoinRoomStruct {
	int roomIndex;
};

// type 204
struct FailJoinRoomStruct {

};

// type 205
struct PermitJoinRoomStruct {
	IN_ADDR hostAddr;
};



// type 206
struct DemandExitRoomStruct {
	int roomIndex;
};

//type 207
struct PermitExitRoomStruct {
	//LobbyInfo
};



//type 208
struct DemandChatStruct {
	char chat[CHAT_BUF_SIZE - 10];
};

// type 209
struct PermitChatStruct {
	char chat[CHAT_MAX_LINE][CHAT_BUF_SIZE];
};


// Demand 서버 -> HOST
// PerMit Host -> 서버

//type 210
struct RequestRoomInfoStruct {

};

//type 211
struct SupplyRoomInfoStruct {
	//RoomData roomData;
};

//type 213
struct UpdateLobbyInfoStruct {
	int m_playersNumber[MAX_ROOM_COUNT]{};
	int m_mapNumber[MAX_ROOM_COUNT]{}; // 맵넘버 1 - 바다, 2 - 숲
};

#pragma endregion

#pragma region [ RoomScene 300 ~ 399 ]

struct RoomInfoStruct {
	int idx;
	SOCKET sock;

	RoomInfoStruct() {}
	RoomInfoStruct(int idx, SOCKET sock) :idx(idx), sock(sock) {}
};

struct QueueData {
	int op;
	int fromIdx;

	QueueData(int op = NULL, int fromIdx = NULL)
		:op(op), fromIdx(fromIdx) {}
};

//type 301
struct UpdateRoomStruct {
	CharacterName m_charInfo[4];
	MapName m_mapInfo;
};

//type 302
struct RequestChangeCharStruct {

};

//type 303
struct PermitChangeCharStruct {
	int m_idx;
	CharacterName m_charInfo;

	PermitChangeCharStruct() {}
	PermitChangeCharStruct(int idx, CharacterName charInfo) : m_idx(idx), m_charInfo(charInfo) {}
};

//type 395
struct SupplyGameStartStruct {

};

//type 396
struct DemandLobbyInfoStruct {

};

//type 397
struct PermitLobbyInfoStruct {

};

//type 398
struct DemandReLoginStruct {

};

//type 399
struct PermitReLoginStruct {

};
#pragma endregion

#pragma region [ RoomScene 400 ~ 499 ]
struct FirstPlatInfo {
	int idx;
	float xPos;
};

struct FirstItemInfo {
	int idx;
	float xPos;
};

struct UseItemInfo {
	int itemNum;
	int userIdx;

	UseItemInfo(int itemNum = NULL, int userIdx = NULL)
		:itemNum(itemNum), userIdx(userIdx) {}
};

//type 401
struct UpdateFrameStruct {
	float m_yPos[MAX_PLAYER];		// 캐릭터 y위치
	float m_totalDis[MAX_PLAYER];			// 캐릭터 속도
	float m_speed[MAX_PLAYER];			// 캐릭터 속도
	State m_state[MAX_PLAYER];			// 캐릭터 현재 상태
	int m_imgCnt[MAX_PLAYER];			// 애니메이션 이미지 카운트
	float m_combo[MAX_PLAYER];			// 캐릭터 콤보 상태
	FirstPlatInfo m_firstPlat[MAX_PLAYER];					// 첫 발판 정보
	FirstItemInfo m_firstItem[MAX_PLAYER];	// 화면보다 오른쪽에 있는 첫 아이템
	int m_itemInfo[MAX_PLAYER][2];		// 보유 아이템 목록
};

//type 402
struct InputJumpStruct {

};

//type 403
struct InputEmotionStruct {

};

//type 404
struct ChangeEmotionStruct {
	int emotionNum;
	int userIdx;

	ChangeEmotionStruct(int emotionNum = NULL, int userIdx = NULL)
		:emotionNum(emotionNum), userIdx(userIdx) {}
};

//type 499
struct SupplyGameEndStruct {

};
#pragma endregion

#pragma endregion

struct UserData {
	char id[5];
	int pw;
	int winCount;
	int loseCount;
};

class Network
{
private:
	SOCKET m_sock;
	
	SceneName m_sceneName;

	int m_sendType{};
	int m_recvType{};
	
	int retVal{};

public:
	Network();
	~Network();

	bool Connect();

	void NetworkThreadFunction();

	void SetSendType(int type) { m_sendType = type; }
	int GetSendType() { return m_sendType; }

	void SetRecvType(int n = 0) { m_recvType = n; }
	int GetRecvType() { return m_recvType; }

	void ChageSceneName(SceneName name) { m_sceneName = name; }
	SOCKET& GetSocket() { return m_sock; }

	int ReturnTypeNumber();
	bool ErrorFunction(int value, int type);
	void err_display(char *msg);
	void err_quit(char *msg);

public:
	UserData myData;
	int      m_roomIndex;

public:
	CRITICAL_SECTION SEND_SECTION;
	CRITICAL_SECTION LOBBY_UPDATE_SECTION;

public:
	//Login Scene	
	DemandLoginStruct* m_demandLogin = NULL;
	PermitLoginStruct* m_permitLogin = NULL;
	
	//Lobby Scene
	DemandChatStruct* m_demandChat = NULL;
	PermitChatStruct* m_permitChat = NULL;

	//InGameScene
	PermitCreateRoomStruct *m_permitCreateRoom = NULL;
	DemandJoinRoomStruct *m_demandJoinRoom = NULL;
	PermitJoinRoomStruct *m_permitJoinRoom = NULL;
	UpdateLobbyInfoStruct* m_updateLobbyInfo = NULL;

public:
#pragma region [SoundManger]
	System*			m_system;

	//	사운드의 개수만큼 필요
	Sound*			m_sound[2];

	//	동시에 출력될 사운드 만큼 필요
	Channel*		m_channel[1];

	//static bool	bPlay = true;				// 플레이 관련 변수 건들지 말기 
	//static bool	IsBGPlaying = false;		// BGM 관련 재생 변수
	bool	bPlay = true;				// 플레이 관련 변수 건들지 말기 
	bool	IsBGPlaying = false;		// BGM 관련 재생 변수

	int		m_nowBgmNumber{};
	void InitSound();
#pragma endregion
};

struct ThreadStruct {
	Network network;
};


extern deque<QueueData> sendQueue[MAX_PLAYER];
extern deque<int> sendQueueGuest;
extern int m_idx;
extern UpdateFrameStruct basicInfo;
extern Pawn* charArr;
extern int	emotionNum[MAX_PLAYER];
extern int  emotionTime[MAX_PLAYER];

extern int numPlayer;
extern UpdateRoomStruct roomInfo;
extern atomic<int> readyPlayer;
extern SOCKET listen_sock;

extern bool gameStart;
extern HANDLE hThreadGuest[2];
extern CharacterName reqChar;
extern deque<UseItemInfo> itemQueue;

DWORD WINAPI ListenThread(LPVOID arg);
DWORD WINAPI RecvData(LPVOID arg);
DWORD WINAPI SendData(LPVOID arg);

DWORD WINAPI RecvDataGuest(LPVOID arg);
DWORD WINAPI SendDataGuest(LPVOID arg);