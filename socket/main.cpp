#include <iostream>
#include <deque>
#include <map>
#include <string>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h> //���߳�


#include <Winsock2.h> //
#include <WS2tcpip.h> //  socket ���
#include <mstcpip.h> //
//#include <boost/shared_ptr.hpp> //boost��
#pragma comment(lib,"ws2_32.lib") //Winsock Library

#include<CLASSsocket.h>
#include<CLASSsocket_server.h>
#include<CLASSsocket_client.h>

using namespace std;

int main() {

	TCP_server TCP;
	TCP.Server_main();
	//TCP_client TCP;
	//TCP.Client_main();

	return 0;
}


