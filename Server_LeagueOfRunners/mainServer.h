#pragma once

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <Windows.h>
#include <fstream>

#include "GameCommuniCationProtocol.h"

#pragma region [Create Static]
#define SERVERIP			"127.0.0.1"
#define SERVERPORT			9000

struct ThreadStruct {
	SOCKET					sock;
	//std::vector<UserData>	threadUserData;	//전역선언함.
};

struct FixDataStruct {
	long long int size;
};

class UserData {
	//basic Data
	char m_id[5]{};
	int m_pw{};
	int m_winCount{};
	int m_loseCount{};

	//Game use Data
	bool m_isLogin{ false };
	IN_ADDR m_userAddr{};

public:
	UserData() {};
	UserData(char ID[5], int PW) { memcpy(m_id, ID, sizeof(ID)); m_pw = PW; m_winCount = 0; m_loseCount = 0; };
	UserData(char ID[5], int PW, int winCount, int loseCount) { memcpy(m_id, ID, sizeof(ID)); m_pw = PW; m_winCount = winCount; m_loseCount = loseCount; };
	~UserData() {};
	
	void	SetIPAddress(IN_ADDR& inputAddress) { m_userAddr = inputAddress; m_isLogin = true; }
	void	PrintUserData() { std::cout << m_id << "  " << m_pw << "  " << m_winCount << "  " << m_loseCount << std::endl; }
	char*	GetID() { return m_id; }
	int		GetPW() { return m_pw; }
	bool	GetIsLogin() { return m_isLogin; }
};

class RoomData {
	int m_playersNumber{};
	int m_mapNumber{}; // 맵넘버 1 - 바다, 2 - 숲
	IN_ADDR m_hostAddr{};

public:
	RoomData() {};
	RoomData(IN_ADDR& inputAddr) { m_hostAddr = inputAddr; m_playersNumber = 1; m_mapNumber = 1; }

	void SetRoomState(int inputPlayerNumber, int inputMapNumber) { m_playersNumber = inputPlayerNumber; m_mapNumber = inputMapNumber; }
	void AddPlayer() { m_playersNumber++; }
};

#pragma endregion

#pragma region [Error Function]

// 소켓 함수 오류 출력 후 종료
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

// 소켓 함수 오류 출력
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

void ErrorFunction(int value, int type) {
	if (type == 0) {
		if (value == SOCKET_ERROR) { err_quit("recv()"); }
		else if (!value) return;
	}
	else if (type == 1) {
		if (value == SOCKET_ERROR) { err_quit("send()"); }
		else if (!value) return;
	}
}
#pragma endregion 

