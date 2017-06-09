#include <iostream>
#define HAVE_STRUCT_TIMESPEC
#include <Winsock2.h> //
#include <WS2tcpip.h> //  socket ¿‡ø‚
#include <mstcpip.h> //
//#include <boost/shared_ptr.hpp> //boostø‚
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;

class CLASSsocket {
protected:
	CLASSsocket::CLASSsocket();
	CLASSsocket::~CLASSsocket();
	SOCKET skt(int type);
	void Model_TCPserver();
	void Model_UDPserver();
	void Model_TCPclient(PCSTR IP,u_short ponte);
	void Model_UDPclient(PCSTR IP, u_short ponte);
	virtual void s_TCPlogic() = 0;
	virtual void c_TCPlogic(sockaddr_in TCPclient_addr) = 0;
	virtual void s_UDPlogic() = 0;
	virtual void c_UDPlogic(sockaddr_in UDPclient_addr) = 0;
	SOCKET S_TCPserver, C_TCPserver, sockConn,S_UDPserver, C_UDPserver;
private:
	WSADATA wsa;
};
