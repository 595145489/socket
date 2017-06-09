#include <CLASSsocket.h>
#include<CLASSsocket_client.h>


TCP_client::TCP_client() {};



void TCP_client::Client_main() {

	
	TCP_client* x = new TCP_client;

	cout << "请输入你在服务器已经注册的名字(按回车表示输入完成,别问为什么!)" << endl;
	getline(cin,x->My_name);
	x->Model_UDPclient("192.168.2.103", 10000);

	pthread_create(&server, NULL, TCP_client::pthread, (void*)x);
	string  send_write;

	while (1) {
		cout << "请输入你要发送好友的名字(按回车表示输入完成,别问为什么!)" << endl;
		getline(cin, x->send_name);

		if (x->send_name == x->My_name) {
			cout << "\n请不要发送给自己哦,亲!\n";
			continue;
		}

		cout << "请输入要发送的内容(按回车表示输入完成,别问为什么!)" << endl;
		getline(cin, send_write);

		x->content.push_back(from_and_send(x->My_name, x->send_name, send_write));
		
		if (x->link_flags == 0) {
			closesocket(x->S_TCPserver);
			x->Model_TCPclient("192.168.2.103",6001);
		}

		if (!x->zhuce) {
			return;
		}

	}
};

string TCP_client::from_and_send(string from_to, string send_to, string write) {

	string a;
	a = "send_to:";
	a += send_to;
	a += "//from_to:";
	a += from_to;
	a += "//content::";
	a += write;
	return a;
}



void* TCP_client::wait_R(void* arg) {

	TCP_client* client = (TCP_client*)arg;

	int revttime = 1000;
	setsockopt(client->C_TCPserver, SOL_SOCKET, SO_RCVTIMEO, (const char*)&revttime, sizeof(int));
	int chaoshiqi = 0;

	char s[1025];



	while (1) {
		int count = recv(client->C_TCPserver, s, 1024, 0);
		if (count > 0) {
			s[count] = 0x00;
			if (strcmp(s,"SEND_OUT")==0) {
				cout << "\n消息发送失败!请检查你是否有这个好友！\n" << endl;
			}
			else{
				cout << s << endl;
			}
			chaoshiqi = 0;
		}
		if (!client->content.empty()) {
			send(client->C_TCPserver, client->content.front().c_str(), strlen(client->content.front().c_str()), 0);
			client->content.pop_front();
			chaoshiqi = 0;
		}
		else {
			chaoshiqi++;
		}
		if (chaoshiqi == 10 && count <= 0) {
			client->link_flags = 0;
			pthread_create(&client->server, NULL, TCP_client::pthread, (void*)client);
			closesocket(client->C_TCPserver);
			return 0;
		}
	}

}

void* TCP_client::pthread(void* arg) {

	TCP_client* client = (TCP_client*)arg;
	client->Model_TCPserver();
	return 0;
};

void TCP_client::s_TCPlogic() {

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);
	char recvBuf[1025];
	string a;
	int link;

	while (1) {

		this->sockConn = accept(this->S_TCPserver, (SOCKADDR*)&addrClient, &len);
		if (this->sockConn == INVALID_SOCKET) {
			return;
		}
		link = recv(sockConn, recvBuf, 1024, 0);
		recvBuf[link] = 0x00;
		a = recvBuf;

		if (a == "link_me") {
			link_flags = 1;
			send(sockConn, "wait_link", strlen("wait_link"), 0);
			closesocket(sockConn);
			closesocket(this->S_TCPserver);
			Model_TCPclient("192.168.2.103",6001);
			return;
		}
	}
};


void TCP_client::c_TCPlogic(sockaddr_in TCPclient_addr) {


	if (connect(this->C_TCPserver, (SOCKADDR*)&TCPclient_addr, sizeof(SOCKADDR))) {
		return;
	}//监听端口


	string x;
	char recvBuf[1024];
	if (link_flags == 0) {//主动连接
						  //char* content = "send_to:link_you//from_to:b//content::send_name";
		x = from_and_send(My_name, "link_you", send_name);
		send(C_TCPserver, x.c_str(), strlen(x.c_str()), 0);
		int count = recv(C_TCPserver, recvBuf,1024,0);

	
		x = recvBuf;
		if (x=="FROM_OUT") {
			cout << "你还没注册，先去注册啊！" << endl;
			this->zhuce = 0;
			closesocket(this->C_TCPserver);
			return;
		}
		else if (x == "SEND_OUT") {
			cout << "消息发送失败!请检查你是否有这个好友！\n\n" << endl;
			this->zhuce = 1;
			closesocket(this->C_TCPserver);
			return;
		}
		else if (x == "SEND_OK") {
			this->zhuce = 1;
		}

		
		link_flags = 2;
	}

	if (link_flags == 1) { //服务器通知连接
						   //char* content = "send_to:link_you//from_to:b//content::-1";
		x = from_and_send(My_name, "link_you", "-1");
		
		send(C_TCPserver, x.c_str() , strlen(x.c_str()), 0);

		this->zhuce = 1;

		link_flags = 2;
	}

	int revttime = 3000;
	setsockopt(this->C_TCPserver, SOL_SOCKET, SO_RCVTIMEO, (const char*)&revttime, sizeof(int));

	//	char* sendBuf;
	char s[1025];
	while (1) {
		int count = recv(C_TCPserver, s, 1024, 0);
		if (count > 0) {
			s[count] = 0x00;
			if (strcmp(s, "SEND_OK") == 0) {
				continue;
			}
			cout << s << endl;	
		}
		else {
			pthread_create(&wait_recv, NULL, TCP_client::wait_R, (void*)this);
			return;
		}

	}
};


void TCP_client::c_UDPlogic(sockaddr_in UDPclient_addr) {

	string a = "name::";
	int len = sizeof(SOCKADDR);
//	char k[1025];
	a += this->My_name;
	sendto(this->C_UDPserver,a.c_str() , strlen(a.c_str()), 0, (SOCKADDR*)&UDPclient_addr, len);
	//int count = recvfrom(this->C_UDPserver, k, 1024, 0, (SOCKADDR *)&UDPclient_addr,&len);
	//int count = recv(this->C_UDPserver,k,1024,0);
	//if (count > 0) {
	//	string q = k;

	//	cout << q << endl;
	//	if (q == "OK") {
	//		this->zhuce = 1;
	//	}
	//	else if (q == "OUT") {
	//		this->zhuce = 0;
	//	}
	//}
	
	closesocket(this->C_UDPserver);
};



void TCP_client::s_UDPlogic() {};