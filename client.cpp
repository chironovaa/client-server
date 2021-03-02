#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <errno.h> 
#include <signal.h> 
#include <strings.h>  
#include <unistd.h> 
#include <iostream> 

#define PORT 5000 
#define MAX 1024

class Client{
public:
	Client(){
		memset(&servaddr, 0, sizeof(servaddr)); 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_port = htons(PORT); 
		servaddr.sin_addr.s_addr = inet_addr("127.0.0.1"); 
	};
	virtual void run() = 0;
protected:
	int socketDescriptor;
	struct sockaddr_in servaddr; 

	virtual void createSocket() = 0;
};

class TCPclient: public Client{
public:
	TCPclient(){
		printf("You choosed TCP port\n");
	};

	void run() override {
		while (true) {
			createSocket();
			connect();

			std::string msg;		
			msg.clear();

			printf("Write a message:\n");
			std::getline(std::cin, msg);
			if (msg == "exit")
				break;
			//send message to Server
			if (send(socketDescriptor, msg.c_str(), sizeof(msg.c_str()), MSG_NOSIGNAL) == -1)
				printf("sending failed");

			char responce[MAX];
			printf("Message from server: \n");
			memset(responce, 0, sizeof(responce));

			//get message from Server
			if(recv(socketDescriptor, responce, sizeof(responce), MSG_NOSIGNAL) == -1)
				printf("getting failed");

			puts(responce);

			close(socketDescriptor);
		}
		close(socketDescriptor);
	};

private:
	void createSocket() override {
		socketDescriptor = socket(AF_INET, SOCK_STREAM, 0);
		if (socketDescriptor < 0) {
			printf("socket creation failed");
			exit(0);
		}
	};
	void connect() {
		if (connect(socketDescriptor, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
			printf("\n Error : Connect Failed \n");
		}
	};
	
};

class UDPclient: public Client{
public:
	UDPclient(){
		printf("You choosed UDP port\n");
	};

	void run() override {
		while (true) {
			createSocket();
			socklen_t len;

			std::string msg;
			printf("Write a message:\n");
			msg.clear();
			std::getline(std::cin, msg);
			if (msg == "exit")
				break;
			//send message to Server
			if (sendto(socketDescriptor, msg.c_str(), strlen(msg.c_str()), 0,
				(const struct sockaddr*)&servaddr, sizeof(servaddr)) == -1) {
				printf("sending failed");
			}
		
			char responce[MAX];
			printf("Message from server: \n");
			//get message from Server
			if(recvfrom(socketDescriptor, (char*)responce, MAX, 0, (struct sockaddr*)&servaddr, &len) == -1)
				printf("getting failed");

			puts(responce);

			close(socketDescriptor);
		}
		close(socketDescriptor);
	};
	
private:
	void createSocket() override {
		socketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
		if (socketDescriptor < 0) {
			printf("socket creation failed");
			exit(0);
		}
	};

};

int main() { 
	char port;
	printf("Port? 1 - TCP, 2 - UDP\n");
	scanf("%c", &port);
	if (port == '2') {
		UDPclient* client = new UDPclient();
		client->run();
	}
	else {
		TCPclient* client = new TCPclient();
		client->run();
	}

	
} 
