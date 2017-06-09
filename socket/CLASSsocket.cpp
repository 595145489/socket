#include <CLASSsocket.h>


CLASSsocket::CLASSsocket() {

	int debug;
	WORD Version = MAKEWORD(2, 2);
	if (debug = WSAStartup(Version, &wsa)) {
		cout << "WSAStartup fail" << endl;
		WSACleanup();
		return;
	}

}
SOCKET CLASSsocket::skt(int type) {
	SOCKET s = socket(AF_INET, type, 0);
	if (s == INVALID_SOCKET) {
		cout << " socket fail" << endl;
		WSACleanup();
	}
	return s;
};

void CLASSsocket::Model_TCPserver() {

	this->S_TCPserver = this->skt(SOCK_STREAM);
	sockaddr_in serverAddr;
	serverAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(6001);

	bind(this->S_TCPserver, (SOCKADDR*)&serverAddr, sizeof(SOCKADDR));


	listen(this->S_TCPserver, 5);


	s_TCPlogic();


}


void CLASSsocket::Model_TCPclient(PCSTR IP, u_short ponte) {

	this->C_TCPserver = this->skt(SOCK_STREAM);
	sockaddr_in goserver;
	inet_pton(AF_INET, IP, (void*)&goserver.sin_addr.S_un.S_addr);
	goserver.sin_family = AF_INET;
	goserver.sin_port = htons(ponte);

	

	c_TCPlogic(goserver);


};



void CLASSsocket::Model_UDPserver() {

	this->S_UDPserver = this->skt(SOCK_DGRAM);
	sockaddr_in UDPserver_addr;
	inet_pton(AF_INET, "0.0.0.0", (void*)&UDPserver_addr.sin_addr.S_un.S_addr);
	UDPserver_addr.sin_family = AF_INET;
	UDPserver_addr.sin_port = htons(10000);
	int len = sizeof(SOCKADDR);
	bind(this->S_UDPserver, (SOCKADDR*)&UDPserver_addr, len);


	s_UDPlogic();

	//sockaddr_in UDPclient;
	//char *sendBUF;
	//char s2[1024];
	//sendBUF = s2;
	//char recvBUF[1025];
	//while (1) {

	//	int count = recvfrom(this->S_UDPserver, recvBUF, 1024, 0, (SOCKADDR *)&UDPclient, &len);
	//	if (count>0) {
	//		recvBUF[count] = 0x00;
	//		cout << recvBUF << endl;
	//	}
	//	char IPdotdec[20];
	//	PCSTR pIPAddr = inet_ntop(AF_INET, &UDPclient.sin_addr, IPdotdec, 16);
	//	cout << pIPAddr << endl;
	//	//cin >> s2;
	//	//sendto(UDPserver, sendBUF, strlen(sendBUF) + 1, 0, (sockaddr*)&UDPclient, len);
	//}

	//closesocket(this->S_UDPserver);
};




void CLASSsocket::Model_UDPclient(PCSTR IP, u_short ponte) {

	this->C_UDPserver = this->skt(SOCK_DGRAM);

	sockaddr_in UDPclient_addr;
	inet_pton(AF_INET, IP, (void*)&UDPclient_addr.sin_addr.S_un.S_addr);
	//UDPclient_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	UDPclient_addr.sin_family = AF_INET;
	UDPclient_addr.sin_port = htons(ponte);

	c_UDPlogic(UDPclient_addr);

}

CLASSsocket::~CLASSsocket() {

	closesocket(this->C_UDPserver);
	closesocket(this->S_UDPserver);
	closesocket(this->S_TCPserver);
	closesocket(this->C_TCPserver);
	WSACleanup();
};