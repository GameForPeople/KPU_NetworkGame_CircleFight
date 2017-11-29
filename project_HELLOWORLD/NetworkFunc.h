#pragma once
#include "stdafx.h"
#include "BaseObject.h"
#include <WinSock2.h>
#include <WS2tcpip.h>
#include "Pawn.h"

#pragma comment(lib, "ws2_32")


#define HOSTPORT 9999
#define ACCEPT_DENIED 18446744073709551615

extern deque<QueueData> sendQueue[MAX_PLAYER];
extern deque<int> sendQueueGuest;
extern int m_idx;
extern BasicInfo basicInfo;
extern Pawn* charArr;
extern int	*emotionNum;
extern int  *emotionTime;

void err_quit(char *msg);
void err_display(char *msg);

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags);