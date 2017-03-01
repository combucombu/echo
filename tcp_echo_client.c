#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 32

struct msg_echo {
	unsigned short seq;
	unsigned short reserve;
	char msg[32];
};

int main(int argc, char* argv[])
{
	int n;
	int sock;
	struct sockaddr_in server;
	struct sockaddr_in client;
	unsigned short serverPort;
	socklen_t fromSize;
	char *serverIP;
	int echoMsgLen;
	int respMsgLen;
	struct msg_echo message;

	if (argc != 3) {
		fprintf(stderr, "Usage: %s <Server IP><Server port>\n", argv[0]);
		exit(1);
	}

	serverIP = argv[1];
	
	if (argc == 3) {
		serverPort = atoi(argv[2]);
	} else {
		serverPort = 7;
	}


	//create a socket
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "sock failed\n");
		exit(2);
	}
	//initialize
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(serverIP);
	server.sin_port = htons(serverPort);

	//connect requset to server
	connect(sock, (struct sockaddr *)&server, sizeof(server));

	//receive date from server
	memset(message.msg, 0, sizeof(message.msg));
	n = read(sock, message.msg, sizeof(message.msg));

	printf("%d, %s\n", n, message.msg);
	
	close(sock);
	return 0;	
}
