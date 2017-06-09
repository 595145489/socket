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
using namespace std;
 
class TCP_client :private CLASSsocket {
public:
	TCP_client::TCP_client();
	void Client_main();

private:
	pthread_t server, wait_recv;
	int link_flags = 0;// 0������״̬ 1�������������Ӳ���������״̬ 2��������״̬  3�Ͽ�ԭ���������¸�ԭ0״̬

	string My_name, send_name;
	bool zhuce=0; // 0������ֹ // 1 ����ɼ��� ����ע��
	deque<string> content;

	void s_TCPlogic();
	void c_TCPlogic(sockaddr_in TCPclient_addr);
	string from_and_send(string from_to, string send_to, string write);
	static void* pthread(void* arg);
	static void* wait_R(void* arg);
	void c_UDPlogic(sockaddr_in UDPclient_addr);
	void s_UDPlogic();
};