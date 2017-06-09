#include <CLASSsocket.h>
#include <CLASSsocket_server.h>


multimap<string, string> TCP_server::message_table;
multimap<string, string>::iterator TCP_server::message_table_iterator;
//deque<string> TCP_server::message[1024];
//deque<string>::iterator TCP_server::pos[1024];
user TCP_server::user_table[1024];



void TCP_server::Server_main() {


	user_table[0].name = "a";
	user_table[1].name = "b";
	user_table[0].sockConn = -1;
	user_table[1].sockConn = -1;
	user_table[0].flag = 0;
	user_table[1].flag = 0;
	user_table[2].name = "c";
	user_table[3].name = "d";
	user_table[2].sockConn = -1;
	user_table[3].sockConn = -1;
	user_table[2].flag = 0;
	user_table[3].flag = 0;
	user_table[0].IP = "127.0.0.1";
	user_table[1].IP = "127.0.0.1";
	user_table[2].IP = "127.0.0.1";
	user_table[3].IP = "127.0.0.1";
	this->Model_UDPserver();
	this->Model_TCPserver();


};

void TCP_server::c_TCPlogic(sockaddr_in TCPclient_addr) {

	if (connect(this->C_TCPserver, (SOCKADDR*)&TCPclient_addr, sizeof(SOCKADDR))) {
		//cout << "对方不在线" << endl;
		return;
	}
	c_RorW(this->C_TCPserver);
};

void TCP_server::s_TCPlogic() {

	SOCKADDR_IN addrClient;
	int len = sizeof(SOCKADDR);

	char recvBuf[1025];
	pthread_canshu parameter;
	int i = -1,j=-1;

//	epoll_event ev, events[20];


	fd_set testfds, readfds;;
	int strat, fd;
	FD_ZERO(&readfds);
	FD_SET(this->S_TCPserver, &readfds);
	FD_SET(this->S_UDPserver, &readfds);
	fdmax = this->S_TCPserver>this->S_UDPserver? this->S_TCPserver: this->S_UDPserver;

	
	message_table.clear();//清空留言数组

	while (1) {
		testfds =readfds;
		strat = select(fdmax + 1, &testfds, (fd_set *)0, (fd_set *)0, (timeval *)0);
		switch (strat)
		{
		case 0:
			break;
		case -1:
			break;
		default:
			for (fd = 0; fd <= fdmax; fd++) {
				if (FD_ISSET(fd, &testfds)) {
					if (fd == this->S_TCPserver) {
						this->sockConn = accept(this->S_TCPserver, (SOCKADDR*)&addrClient, &len);
						if (this->sockConn == -1) {
							cout << "this->sockConn==-1la" << endl;
							break;
						}
						
						FD_SET(this->sockConn, &readfds);
						if (fdmax<this->sockConn) {
							fdmax = this->sockConn;
						}
						cout << "link_OK" << endl;
						break;
					}
					else if (fd == this->S_UDPserver) {
						sockaddr_in UDPclient;
						char recvBUF[1025];
						char IPdotdec[20];
						int x=0;
						string name;
						//name::a
						int count = recvfrom(this->S_UDPserver, recvBUF, 1024, 0, (SOCKADDR *)&UDPclient, &len);
						if (count > 0) {
							name = recvBUF;
							name = name.substr(6, 1);
						}
						else
							continue;

						PCSTR pIPAddr = inet_ntop(AF_INET, &UDPclient.sin_addr, IPdotdec, 16);
						//cout << name << endl;
						while (1) {
							if (user_table[x].name == name) {
								user_table[x].IP = pIPAddr;
								break;
							}
							if (x>=1023) {
								break;
							}
							++x;
						}
						

					}
					else {
						int count = recv(fd, recvBuf, 1024, 0);
						if (count > 0) {
							recvBuf[count] = 0x00;
							//植入线程
							parameter.recvString = recvBuf;
							parameter.sockconn = fd;
							pthread_create(&pthread_c[++i], NULL, TCP_server::pthread, (void*)&parameter);
							i %= 1023;
						}
						else {

							cout << "";  //不知道为什么这里需要加个cout 要不下面没用
 							int a;
							a = fd;
							FD_CLR(fd, &readfds);
							pthread_create(&pthread_cls[++j], NULL, TCP_server::pthread_close, (void*)&a);
							closesocket(fd);						
							j %= 1023;
						}
					}
				}//end if FD_ISSET
			} //end for
		}
	}// end whlie
};

void*  TCP_server::pthread_close(void* arg) {

	
	int a = *(int *)arg;

	int x = 0;
	while (1) {
		if (user_table[x].sockConn == a) {
			user_table[x].sockConn = -1;
			user_table[x].flag = 0;
			cout << "close OK" << endl;
			break;
		}
		if (x >= 1023) {
			
			return 0;
		}
		++x;
	}
	return 0;
}

void*  TCP_server::pthread(void* arg) {

	pthread_canshu parameter = *(pthread_canshu *)arg;
	TCP_server	TCP_class;
	user_send num;
	int x=0,y=0;
	u_short q=6001;
	num  = send_who(&parameter.recvString);


	// 第一条收到来自客户端有关我的信息
	if (num.send_to == "link_you" ) {
		while (1) {
			if (user_table[x].name == num.from_to) {
				user_table[x].flag = 1;
				user_table[x].sockConn = parameter.sockconn;	
				break;
			}	
			if (x >= 1023) {
				send(parameter.sockconn,"FROM_OUT",strlen("FROM_OUT")+1,0);
				return 0;
			}
			++x;
		}

		if (parameter.recvString != "-1") {//如果是返回的连接则不需要以下这步  
			while (1) {
				if (user_table[y].name == parameter.recvString ) {
					if (user_table[y].flag == 0) {
						
						if (parameter.recvString == "a") q = 6002;
						if (parameter.recvString == "b") q = 6003;
						//if (parameter.recvString == "c") q = 6004;
						//if (parameter.recvString == "d") q = 6005;
						TCP_class.Model_TCPclient(user_table[y].IP, q); // 建立服务器与客户端的一次连接
				
					}
					break;
				}
				if (y >= 1023) {
					send(parameter.sockconn, "SEND_OUT", strlen("SEND_OUT") + 1, 0);
					return 0;
				}
				++y;
			}	
		}

		send(parameter.sockconn, "SEND_OK", strlen("SEND_OK") + 1, 0);
		while (1) {
			message_table_iterator = message_table.find(user_table[x].name);
			if (message_table_iterator != message_table.end()) {
				send(user_table[x].sockConn, message_table_iterator->second.c_str(), strlen(message_table_iterator->second.c_str()), 0);
				message_table.erase(message_table_iterator);
			}
			else
				break;
		}
		return 0;
	}

	TCP_server::s_W(parameter, num);
	return 0;

}

void TCP_server::s_W(pthread_canshu parameter, user_send num) {

	int x = 0;
	while (1) {
		if (user_table[x].name == num.send_to) {		
			break;
		}
		if (x>=1023) {
			send(parameter.sockconn, "SEND_OUT", strlen("SEND_OUT") + 1, 0);
			return;
		}
		x++;
	}

	
	string a = "来自:";
	a += num.from_to;
	a += "      信息:";
	a += parameter.recvString;

	if (user_table[x].flag == 1) {//如果send_to在线

		send(user_table[x].sockConn,a.c_str(),strlen(a.c_str()), 0);
	}
	else {//不在线  //将东西存储在表单中
		
		message_table.insert(pair<string, string>(user_table[x].name, a));
	}
	return;
}


void TCP_server::c_RorW(SOCKET sockConn) {

	char recvBuf[1025];
	char *sendBuf = "link_me";
	do {
		int x = send(sockConn, sendBuf, strlen(sendBuf), 0);

		int count = recv(sockConn, recvBuf, 1024, 0);
		//if (count > 0) {

		//}
		//else {

		//}
	} while (0);
	closesocket(sockConn);
	return;
};





user_send  TCP_server::send_who(string *recfBuf_string) {

	user_send us;
	//对字符串进行解析,在这个地方分解出from where &  send where 
	//string recfBuf_string = "send_to:aa//from_to:bb//content::zkdcbsldjkhbsdfbvsoidfbsodifb";
	size_t iPos = recfBuf_string->find("//");
	string send_to = recfBuf_string->substr(0, iPos);
	recfBuf_string->erase(0, iPos + 2);
	iPos = recfBuf_string->find("//");
	string from_to = recfBuf_string->substr(0, iPos);
	recfBuf_string->erase(0, iPos + 2);
	iPos = recfBuf_string->find("::");
	recfBuf_string->erase(0, iPos + 2);
	iPos = send_to.find(":");
	send_to.erase(0, ++iPos);
	iPos = from_to.find(":");
	from_to.erase(0, ++iPos);
	us.from_to = from_to;
	us.send_to = send_to;
	return us;

};

void TCP_server::c_UDPlogic(sockaddr_in UDPclient_addr) {};
void TCP_server::s_UDPlogic() {};













