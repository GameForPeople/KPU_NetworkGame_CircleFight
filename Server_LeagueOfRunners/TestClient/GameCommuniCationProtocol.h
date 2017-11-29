#pragma once

#pragma region [ GAME COMMUNICATION PROTOCOL]

int ReturnTypeNumber(SOCKET& clientSock) {
	int type, retVal;

	retVal = recv(clientSock, (char*)&type, sizeof(type), 0);
//	ErrorFunction(retVal, 0);
	return type;
}

enum Protocol {
	DEMAND_LOGIN = 100
	, FAIL_LOGIN = 101
	, PERMIT_LOGIN = 102
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

};



// type 206
struct DemandLobbyInfoStruct {

};

//type 207
struct PermitLobbyInfoStruct {
	//LobbyInfo
};



//type 208
struct DemandChatStruct {
	char chat[255];
};

// type 209
struct PermitChatStruct {
	char chat[5][255];
};


// Demand ���� -> HOST
// PerMit Host -> ����

//type 210
struct DemandRoomInfoStruct {

};

//type 211
struct PermitRoomInfoStruct {
	RoomData roomData;
};

#pragma endregion
