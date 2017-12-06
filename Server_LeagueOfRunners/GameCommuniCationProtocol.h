#pragma once

#include "mainServer.h"

#pragma region [Error Function]

// ���� �Լ� ���� ��� �� ����
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// ���� �Լ� ���� ���
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

bool ErrorFunction(int value, int type) //type = 0 recv, type = 1send
{
	if (type == 0) {
		if (value == SOCKET_ERROR) { err_display("recv()"); return false; } // ������ �ϰ� �̻���..�� �׷��� Ŭ�� ���Ӳ����ž�
		else if (!value) return false;	// ������ ������ �̻���. �׷��� Ŭ��� ��䳪��
		else return true;	//������.
	}
	else if (type == 1) {
		if (value == SOCKET_ERROR) { err_display("send()"); return false; }	// �����ϸ� ���� �̻��� �̳�. �׷��� Ŭ�� ���Ӳ����ž�
		else if (!value) return false;	// 0...? �̷��� ���Ͼ.	�¹��� Ŭ��
		else return true;	// ������.
	}
}
#pragma endregion 

#pragma region [ GAME COMMUNICATION PROTOCOL]

int ReturnTypeNumber(SOCKET& clientSock) {
	int type, retVal;

	//std::cout << "   ToDebug : �����͸� ���������մϴ�." << std::endl;
	retVal = recv(clientSock, (char*)&type, sizeof(type), 0);
	if (!ErrorFunction(retVal, 0))
		return 0;
	//std::cout << "   ToDebug : �����͸� ���۹޾ҽ��ϴ�.." << std::endl;

	return type;
}

enum Protocol{ 
	//LoginScene
		DEMAND_LOGIN		=	100
	,	FAIL_LOGIN			=	101
	,	PERMIT_LOGIN		=	102

	//LobbyScene
	,	DEMAND_CREATEROOM	=	200
	,	FAIL_CREATEROOM		=	201
	,	PERMIT_CREATEROOM	=	202
	
	,	DEMAND_JOINROOM		=	203
	,	FAIL_JOINROOM		=	204
	,	PERMIT_JOINROOM		=	205
	
	,	DEMAND_LOBBYINFO	=	206
	,	PERMIT_LOBBYINFO	=	207
	
	,	DEMAND_CHAT			=	208
	,	PERMIT_CHAT			=	209

	,	REQUEST_ROOMINFO	=	210 //Server -> Host  
	,	SUPPLY_ROOMINFO		=	211	//Host	 -> Server


	,	UPDATE_LOBBY		=	212
	//RoomScene
	,	SUPPLY_GAMESTART	=	395 //HOST	-> Server
	
	,	DEMAND_EXITROOM		=	396 //Host -> Server
	,	PERMIT_EXITROOM		=	397 //Server -> Host

	,	DEMAND_RELOGIN		=	398	//GUEST	-> Server	
	,	PERMIT_RELOGIN		=	399 //
		

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

	,   DEMAND_SENDRESULT   =   498
	,	SUPPLY_GAMEEND		=	499	//HOST	-> Server
};

enum class SceneName {
	Title
	, Login
	, Lobby
	, Room
	, RoomGuest
	, InGame
	, InGameGuest
};

#pragma endregion

#pragma region [ LoginScene 100 ~ 199 ]

//type 100 Client -> Server
struct DemandLoginStruct {
	int type{};	// 1�϶��� �α���, 2�϶��� ȸ������
	int PW{};
	char ID[5];
};

// type 101 Server -> Client �α��� ����, ȸ������ ����
struct FailLoginStruct {
};

// type 102 Server -> Client �α��� ����, Lobby����, �������� ����
struct PermitLoginStruct {
	int winCount{};
	int loseCount{};
	// LobbyInfoData
};
#pragma endregion

#pragma region [ LobbyScene	200 ~ 299]

//Demand Ŭ�� -> ����
//Fail, Permit ���� -> Ŭ��

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
struct DemandExitRoomStruct{
	int roomIndex;
};

//type 207
struct PermitExitRoomStruct {
	//LobbyInfo
};



//type 208
struct DemandChatStruct {
	char chat[CHAT_BUF_SIZE];
};

// type 209
struct PermitChatStruct {
	char chat[CHAT_MAX_LINE][CHAT_BUF_SIZE];
};


// Demand ���� -> HOST
// PerMit Host -> ����

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
	int m_mapNumber[MAX_ROOM_COUNT]{}; // �ʳѹ� 1 - �ٴ�, 2 - ��
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

#pragma region [ InGameScene 400 ~ 499 ]

//type 499
struct SupplyGameEndStruct {

};

#pragma endregion