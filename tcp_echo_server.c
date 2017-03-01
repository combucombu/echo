#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ECHOMAX 32

struct echo_msg {
	unsigned short seq;
	unsigned short reserve;
	char msg[32];
};

int main(int argc, char* argv[])
{
	int n;
	int sock0, sock;
	struct sockaddr_in server;
	struct sockaddr_in client;
	unsigned int cliAddrLen;
	unsigned short serverPort;
	int recvMsgSize;
	struct echo_msg message;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <TCP SERVER PORT>\n", argv[0]);
		exit(1);
	}
	//get server port number
	serverPort = atoi(argv[1]);
	
	//create a socket
	if ((sock0 = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		fprintf(stderr, "sock failed\n");
		exit(2);
	}
	//initialize
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = htonl(INADDR_ANY);
	server.sin_port = htons(serverPort);

	//bind server addr to the socket
	if (bind(sock0, (struct sockaddr *) &server, sizeof(server)) < 0) {
		fprintf(stderr, "bind failed\n");
		exit(3);
	}

	//wait connection request
	listen(sock0, 5);

	//accept connection request
	cliAddrLen = sizeof(client);
	sock = accept(sock0, (struct sockaddr *)&client, &cliAddrLen); 
	fprintf(stdout, "connected client %s\n", 
			inet_ntoa(client.sin_addr));
	write(sock, "HELLO", 5);

	for (;;) {
		//receive message
		n = read(sock, &message, sizeof(message));
		fprintf(stdout, "message = %s\n", message.msg);
		//increment seq
		message.seq++;
		//send message
		write(sock, &message, sizeof(message));
	}
	close(sock);
	close(sock0);

	return 0;
}
