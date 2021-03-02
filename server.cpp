#include <arpa/inet.h> 
#include <errno.h> 
#include <netinet/in.h> 
#include <signal.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <unistd.h> 
#include <algorithm>
#include <string>
#include <sstream> 
#include <stack>
#include <iostream> 
#define PORT 5000 
#define MAXLINE 1024 

class Server{
public:
	Server(){
		bzero(&servaddr, sizeof(servaddr)); 
		servaddr.sin_family = AF_INET; 
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
		servaddr.sin_port = htons(PORT); 
	};
	void run(){
		fd_set rset; 
		if (!createTCPSocket()){
			 perror("socket failed"); 
        	exit(EXIT_FAILURE);
		}
		bindServerToTCP();
		if (!createUDPSocket()){
			 perror("socket failed"); 
        	exit(EXIT_FAILURE);
		}
		bindServerToUDP();
		FD_ZERO(&rset); 
		char msg[MAXLINE];
		for (;;) { 
			// set TCPsocketDescriptor and UDPsocketDescriptor in readset 
			FD_SET(TCPsocketDescriptor, &rset); 
			FD_SET(UDPsocketDescriptor, &rset); 

			select(std::max(TCPsocketDescriptor, UDPsocketDescriptor) + 1, &rset, NULL, NULL, NULL);
			// if tcp socket is readable then handle 
			// it by accepting the connection 
			if (FD_ISSET(TCPsocketDescriptor, &rset)) { 
				 chatTCP();
			} 
			// if udp socket is readable receive the message. 
			if (FD_ISSET(UDPsocketDescriptor, &rset)) { 
				chatUDP();
			} 
		} 
	};
private:
	int TCPsocketDescriptor, UDPsocketDescriptor;
	struct sockaddr_in cliaddr, servaddr; 

	const char* handle(const char str[MAXLINE]) const {
		std::stack<int> numbers;
		int sum = 0;

		std::stringstream ss;
		ss << str;
		std::string temp;
		int found;
		while (!ss.eof()) {
			ss >> temp;
			if (std::stringstream(temp) >> found) {
				numbers.push(found);
				sum += found;
			}
		}
		if (numbers.empty())
			return str;
		else {
			std::string newS;
			int num;
			while (!numbers.empty()) {
				newS.append(std::to_string(numbers.top()) + " ");
				numbers.pop();
			}
			newS.append("\n" + std::to_string(sum));
			return newS.c_str();
		}
	};

	bool createTCPSocket(){
		TCPsocketDescriptor = socket(AF_INET, SOCK_STREAM, 0); 
		return TCPsocketDescriptor >= 0;//если <0 то socked failed
	};
	bool bindServerToTCP(){
		bind(TCPsocketDescriptor, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
		listen(TCPsocketDescriptor, SOMAXCONN); 
		return true;
	};
	
	bool createUDPSocket(){
		UDPsocketDescriptor = socket(AF_INET, SOCK_DGRAM, 0);
		return UDPsocketDescriptor >= 0;//если 0 то socked failed
	};
	bool bindServerToUDP(){
		bind(UDPsocketDescriptor, (struct sockaddr*)&servaddr, sizeof(servaddr)); 
		return true;
	};
	
	void chatTCP(){
		char buffer[MAXLINE];
		socklen_t len = sizeof(cliaddr); 
		int connfd = accept(TCPsocketDescriptor, (struct sockaddr*)&cliaddr, &len); 
		if (fork() == 0) { 
			close(TCPsocketDescriptor); 

			bzero(buffer, sizeof(buffer)); 
			printf("Message From TCP client: \n"); 
			recv(connfd, buffer, sizeof(buffer), MSG_NOSIGNAL); 
			puts(buffer); 

			send(connfd, handle(buffer), sizeof(buffer), MSG_NOSIGNAL);

			close(connfd); 
			exit(0); 
		} 
		close(connfd);
	};
	void chatUDP(){
		char buffer[MAXLINE];
		socklen_t len = sizeof(cliaddr); 
		bzero(buffer, sizeof(buffer)); 
		printf("\nMessage from UDP client: \n"); 
		recvfrom(UDPsocketDescriptor, buffer, sizeof(buffer), 0, 
							(struct sockaddr*)&cliaddr, &len); 
		puts(buffer); 
		sendto(UDPsocketDescriptor, handle(buffer), sizeof(buffer), 0,
					(struct sockaddr*)&cliaddr, sizeof(cliaddr)); 
	};
};

int main() { 
	Server *server = new Server();
	server->run();
} 
