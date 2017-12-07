#pragma once

#include <winsock2.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <Windows.h>
#include <thread>
#include <atomic>

//#include "GameCommuniCationProtocol.h"

//#define DEBUG_MODE //print Debug Message! 

#pragma region [Create Static]
#define SERVERIP			"127.0.0.1"
#define SERVERPORT			9000

#define CHAT_BUF_SIZE		30
#define CHAT_MAX_LINE		5

#define MAX_ROOM_COUNT		8


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
	int		GetWinCount() { return m_winCount; }
	int		GetLoseCount() { return m_loseCount; }

	void	SetWinOrLose(int value) {
		if (value == 1) { m_winCount++; }
		else if (value == 2) { m_loseCount++; }
		return;
			}

	bool	GetIsLogin() { return m_isLogin; }
	void	SetIsLogin(bool bValue) { m_isLogin = bValue; }
};

class RoomData {
	bool m_isCreate{false};
	int m_playersNumber{};
	int m_mapNumber{}; // 맵넘버 1 - 바다, 2 - 숲
	IN_ADDR m_hostAddr{};

public:
	RoomData() { m_isCreate = false; }
	RoomData(IN_ADDR& inputAddr) { m_hostAddr = inputAddr; m_playersNumber = 1; m_mapNumber = 1; }

	void Create(IN_ADDR& inputAddr) { m_hostAddr = inputAddr; m_playersNumber = 1; m_mapNumber = 1; m_isCreate = true; }
	bool Destory() { if (m_isCreate) { m_isCreate = false; m_playersNumber = 0; return true; } else return false; }
	int Exit() { 
		if (m_isCreate) {
			m_playersNumber--; 

			if (m_playersNumber == 0) {
				m_isCreate = false;
				return 2;
			}

			return 1; 
		} 
		else 
			return 0; 
	}

	bool Join() { if (m_isCreate && m_playersNumber < 4) { m_playersNumber++; return true;  } else return false; }

	void AddPlayer() { m_playersNumber++; }
	void SetRoomState(int inputPlayerNumber, int inputMapNumber) { m_playersNumber = inputPlayerNumber; m_mapNumber = inputMapNumber; }
	
	int GetPlayersNumber() { return m_playersNumber; }
	int GetMapNumber() { return m_mapNumber; }

	bool GetIsCreate() { return m_isCreate; }
	IN_ADDR& GetHostAddr() { return m_hostAddr; }
};

class LobbyInfo {
private:
	//std::vector<RoomData>	m_roomArr;
	RoomData					m_roomArr[MAX_ROOM_COUNT];

	int							m_roomCount{};

public:
	TCHAR	m_chatBuf[5][CHAT_BUF_SIZE] = { NULL };
	TCHAR	m_chat[CHAT_BUF_SIZE] = { NULL };

	LobbyInfo() = default;
	~LobbyInfo() = default;

	int CreateRoom(IN_ADDR& hostAddr){
		if (m_roomCount < 8) {
			for (int i = 0; i < MAX_ROOM_COUNT; i++) {
				if (!m_roomArr[i].GetIsCreate()) {
					m_roomArr[i].Create(hostAddr);
					m_roomCount++;
					//std::cout << i << "번방시도" << std::endl;
					return i;
				}
			}
		}
		else if (m_roomCount >= 8) return -1;
	}

	bool ExitRoom(int roomIndex) {
		//m_roomCount--;
		if( m_roomArr[roomIndex].Exit() == 2) m_roomCount--;
		return 1;
	}

	bool DestoryRoom(int roomIndex) {
		m_roomCount--;
		return m_roomArr[roomIndex].Destory();
	}

	bool JoinRoom(int roomIndex, IN_ADDR& outAddr) {
		outAddr = m_roomArr[roomIndex].GetHostAddr();
		return(m_roomArr[roomIndex].Join());
	}

	void PushChat() {
		for (int i = 1; i < CHAT_MAX_LINE; i++) {
		    for ( int j = 0; j < CHAT_BUF_SIZE ; j++)
				m_chatBuf[i - 1][j] =  m_chatBuf[i][j];
			//std::cout << m_chatBuf[i - 1] << std::endl;
		}
	}
	void Chat(char* recvChat) {
		for (int i = 1; i < CHAT_MAX_LINE; i++) {
			memcpy(m_chatBuf[i - 1], m_chatBuf[i], sizeof(CHAT_BUF_SIZE));
			
			#ifdef DEBUG_MODE
			std::cout << m_chatBuf[i - 1] << std::endl;
			#endif
		}

		memcpy(m_chatBuf[CHAT_MAX_LINE - 1], recvChat, sizeof(CHAT_BUF_SIZE));
		
		#ifdef DEBUG_MODE
		std::cout << m_chatBuf[CHAT_MAX_LINE - 1] << std::endl;
		#endif		
	}
	char* GetChatBuf(int index) { return &(m_chat[index]); };

	int	GetRoomPlayersNumber(int index) {
		return m_roomArr[index].GetPlayersNumber();
	}
	int	GetRoomMapNumber(int index) {
		return m_roomArr[index].GetMapNumber();
	}
	//int GetRoomCount
};

#pragma endregion

