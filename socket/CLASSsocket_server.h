#include <iostream>
#include <map>
#include<cstdlib>
//#include<epoll.h>
#include <string>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h> //多线程
#include <Winsock2.h> //
#include <WS2tcpip.h> //  socket 类库
#include <mstcpip.h> //
//#include <boost/shared_ptr.hpp> //boost库
#pragma comment(lib,"ws2_32.lib") //Winsock Library
using namespace std;



struct pthread_canshu {

	string recvString;
	int sockconn;
};

struct user_send {
	string from_to;
	string send_to;
};


struct user {
	string name; //send_to from to 的地址;
	bool flag; //是否在线 0不在线 
	int sockConn; // socket号
	PCSTR IP;  //IP号
};



class TCP_server : private CLASSsocket {
public:
	TCP_server::TCP_server() {};
	void TCP_server::Server_main();
private:
	int fdmax;
	pthread_t pthread_c[1024],pthread_cls[1024];

	//static deque<string> message[1024];
	//static deque<string>::iterator pos[1024];
	static multimap<string, string> message_table;
	static multimap<string, string>::iterator TCP_server::message_table_iterator;
	//int fd;

	static user user_table[1024];

	void c_RorW(SOCKET sockConn);
	static void s_W(pthread_canshu parameter, user_send num);
	static void* pthread(void* arg);
	static void* pthread_close(void* arg);
	static user_send send_who(string *recfBuf_string);
	void s_TCPlogic();
	void c_TCPlogic(sockaddr_in TCPclient_addr);
	void c_UDPlogic(sockaddr_in UDPclient_addr);
	void s_UDPlogic();
};