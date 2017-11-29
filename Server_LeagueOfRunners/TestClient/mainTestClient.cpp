#pragma comment(lib, "ws2_32")

#include "main.h"

int main(int argc, char *argv[])
{
	int retVal;

#pragma region [// 윈속 초기화]
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;
#pragma endregion

#pragma region [ socket() ]
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
#pragma endregion

	std::cout << "--------------------------------------------------- " << std::endl;
	std::cout << "And When We Advance			 // TESTCLIENT		  " << std::endl;
	std::cout << "                                ver 0.2  171121     " << std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;

#pragma region [ connect() ]
	static SOCKADDR_IN serverAddr;
	ZeroMemory(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serverAddr.sin_port = htons(SERVERPORT);
	retVal = connect(sock, (SOCKADDR *)&serverAddr, sizeof(serverAddr));
	if (retVal == SOCKET_ERROR) err_quit("bind()");
#pragma endregion

#pragma region [Print ClientUI]

	std::cout << "   테스트 클라이언트가 서버에 접속하였습니다.     " << std::endl;
	std::cout << "   PORT NUMBER : " << serverAddr.sin_port				<< std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;

#pragma endregion

#pragma region [TestFunction]
	//recv(sock, )

	while(7){
	int inputOption{};

	std::cout << "테스트를 시작합니다. 1은 Send, 2는 Recv, 3은 Break  --->  ";

	std::cin >> inputOption;
	if (inputOption == 3) break;

	if (inputOption == 1) {
		std::cout << "  Testing Client Send - Server Recv, Choice Protocol " << std::endl;
		int inputProtocol{};
		std::cout << "  원하시는 ProtocolNumber or Datasize를 입력해주세요 ---> ";
		std::cin >> inputProtocol;
		
		if(inputProtocol >= 100)
			send(sock, (char*)&inputProtocol, sizeof(inputProtocol), 0);
		else if (inputProtocol < 100) {
			std::vector<char> sendBuffer;
			for (int i = 0; i < inputProtocol; i++) {
				sendBuffer.emplace_back('a');
			}

			send(sock, (char*)&sendBuffer, sizeof(inputProtocol), 0);
		}

		std::cout << "  정상적으로 Protocol Number를 전송했습니다. " << std::endl;
	}
	else if (inputOption == 2) {
		int inputProtocolNumber{};
		std::cout << "  Testing Client Recv - Server Send, Choice Protocol ";
		std::cout << "  OnReceving ";

		int recvProtocolNumber{};
		recv(sock, (char*)&recvProtocolNumber, sizeof(recvProtocolNumber), 0);

		if (recvProtocolNumber == DEMAND_LOGIN) {
			DemandLoginStruct bufferStruct;
			recv(sock, (char*)&bufferStruct, sizeof(bufferStruct), 0);
			//::cout <<"전송받은 정보는 "
		}
		else if (recvProtocolNumber == FAIL_LOGIN) {
			FailLoginStruct bufferStruct;
			recv(sock, (char*)&bufferStruct, sizeof(bufferStruct), 0);
			std::cout << "전송받은 정보는 " << "로그인에 실패했다는 정보입니다." << std::endl;
		}
		else if (recvProtocolNumber == PERMIT_LOGIN) {
			PermitLoginStruct bufferStruct;
			recv(sock, (char*)&bufferStruct, sizeof(bufferStruct), 0);
			std::cout << "전송받은 정보는 " << "PerMitLogin"
				<< "  1. winCount :" << bufferStruct.winCount
				<< "  2. loseCount :" << bufferStruct.loseCount
				<< std::endl;
		}
		}
	}

#pragma endregion

#pragma region [ GoodBye Server! ]
	std::cout << "---Master- 서버를 종료하시겠습니까? --> ";
	char buf{};
	std::cin >> buf;


	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();

	return 0;
#pragma endregion
}