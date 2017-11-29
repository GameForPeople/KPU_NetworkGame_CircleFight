#pragma comment(lib, "ws2_32")

#include "main.h"

int main(int argc, char *argv[])
{
	int retVal;

#pragma region [// ���� �ʱ�ȭ]
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

	std::cout << "   �׽�Ʈ Ŭ���̾�Ʈ�� ������ �����Ͽ����ϴ�.     " << std::endl;
	std::cout << "   PORT NUMBER : " << serverAddr.sin_port				<< std::endl;
	std::cout << "--------------------------------------------------  " << std::endl;

#pragma endregion

#pragma region [TestFunction]
	//recv(sock, )

	while(7){
	int inputOption{};

	std::cout << "�׽�Ʈ�� �����մϴ�. 1�� Send, 2�� Recv, 3�� Break  --->  ";

	std::cin >> inputOption;
	if (inputOption == 3) break;

	if (inputOption == 1) {
		std::cout << "  Testing Client Send - Server Recv, Choice Protocol " << std::endl;
		int inputProtocol{};
		std::cout << "  ���Ͻô� ProtocolNumber or Datasize�� �Է����ּ��� ---> ";
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

		std::cout << "  ���������� Protocol Number�� �����߽��ϴ�. " << std::endl;
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
			//::cout <<"���۹��� ������ "
		}
		else if (recvProtocolNumber == FAIL_LOGIN) {
			FailLoginStruct bufferStruct;
			recv(sock, (char*)&bufferStruct, sizeof(bufferStruct), 0);
			std::cout << "���۹��� ������ " << "�α��ο� �����ߴٴ� �����Դϴ�." << std::endl;
		}
		else if (recvProtocolNumber == PERMIT_LOGIN) {
			PermitLoginStruct bufferStruct;
			recv(sock, (char*)&bufferStruct, sizeof(bufferStruct), 0);
			std::cout << "���۹��� ������ " << "PerMitLogin"
				<< "  1. winCount :" << bufferStruct.winCount
				<< "  2. loseCount :" << bufferStruct.loseCount
				<< std::endl;
		}
		}
	}

#pragma endregion

#pragma region [ GoodBye Server! ]
	std::cout << "---Master- ������ �����Ͻðڽ��ϱ�? --> ";
	char buf{};
	std::cin >> buf;


	// closesocket()
	closesocket(sock);

	// ���� ����
	WSACleanup();

	return 0;
#pragma endregion
}