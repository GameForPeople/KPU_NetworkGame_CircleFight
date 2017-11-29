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


// Demand 서버 -> HOST
// PerMit Host -> 서버

//type 210
struct DemandRoomInfoStruct {

};

//type 211
struct PermitRoomInfoStruct {
	RoomData roomData;
};

#pragma endregion
