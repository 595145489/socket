#include <iostream>
#include <deque>
#include <map>
#include <string>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h> //多线程
#include <Winsock2.h> //
#include <WS2tcpip.h> //  socket 类库
#include <mstcpip.h> //
//#include <boost/shared_ptr.hpp> //boost库
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;
 
class TCP_client :private CLASSsocket {
public:
	TCP_client::TCP_client();
	void Client_main();

private:
	pthread_t server, wait_recv;
	int link_flags = 0;// 0无连接状态 1服务器请求连接并被动连接状态 2保持连接状态  3断开原来连接重新复原0状态

	string My_name, send_name;
	bool zhuce=0; // 0程序终止 // 1 程序可继续 即已注册
	deque<string> content;

	void s_TCPlogic();
	void c_TCPlogic(sockaddr_in TCPclient_addr);
	string from_and_send(string from_to, string send_to, string write);
	static void* pthread(void* arg);
	static void* wait_R(void* arg);
	void c_UDPlogic(sockaddr_in UDPclient_addr);
	void s_UDPlogic();
};