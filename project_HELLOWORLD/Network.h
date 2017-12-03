#pragma once

#pragma comment(lib, "ws2_32")

#include "stdafx.h"

#define CHAT_BUF_SIZE		30
#define CHAT_MAX_LINE		5
#define MAX_ROOM_COUNT		8


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
	, SUPPLY_GAMESTART = 395 //HOST	-> Server

	, DEMAND_EXITROOM = 396 //Host -> Server
	, PERMIT_EXITROOM = 397 //Server -> Host

	, DEMAND_RELOGIN = 398	//GUEST	-> Server	
	, PERMIT_RELOGIN = 399 //


						   //INGAMESCENE
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
};

struct ThreadStruct {
	Network network;
};


