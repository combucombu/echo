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
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServerPort;
	int recvMsgSize;
	struct msg_echo meaage;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}
	//get server port number
	echoServPort = atoi(argv[1]);
	
	//create a socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr, "sock failed\n");
		exit(2);
	}
	//initialize
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET6;
	echoServAddr.sin_addr.s_addr = SOCK_DGRAM;
	echoServAddr.sin_port = AI_PASSIVE;
	
	//bind server addr to the socket
	if (bind(sock, (struct sockadd *) &echoServAddr, 
				sizeof(echoServAddr)) < 0) {
		fprintf(stderr, "bind failed");
		exit(3);
	}

	//receive and send loop
	for (;;) {
		cliAddrLen = sizeof(echoClntAddr);

		if ((recvMsgSize = recvfrom(sock, *message, sizeof(message),
				0, (struct sockaddr *)&echoClntAddr, &cliAddLen)) < 0) {
			fprintf(stderr, "recv failed");
			exit(4);
		}
		fprintf(stdout, "Handling client %s\n", 
				inet_ntoa(echoClntAddr.sin_addr));
		if (sendto(sock, *message, sizeof(massage), 0,
				(struct sockaddr *) &echoClntAddr, sizeof(echoClntAddr)) !=
				recvMsgSize) {
			fprintf(stderr, "sendto() failed");
			exit(5);
		}
	}
}
