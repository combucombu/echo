#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
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
	int sock;
	struct sockaddr_in echoServAddr;
	struct sockaddr_in echoClntAddr;
	unsigned int cliAddrLen;
	char echoBuffer[ECHOMAX];
	unsigned short echoServPort;
	int recvMsgSize;
	struct echo_msg message;

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <UDP SERVER PORT>\n", argv[0]);
		exit(1);
	}
	//get server port number
	echoServPort = atoi(argv[1]);
	
	while(1) {
		//create a socket
		if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
			fprintf(stderr, "sock failed\n");
			exit(2);
		}
		//initialize
		memset(&echoServAddr, 0, sizeof(echoServAddr));
		echoServAddr.sin_family = AF_INET;
		echoServAddr.sin_addr.s_addr = htonl(INADDR_ANY);
		echoServAddr.sin_port = htons(echoServPort);
	
		//bind server addr to the socket
		if (bind(sock, (struct sockaddr *) &echoServAddr, 
					sizeof(echoServAddr)) < 0) {
			fprintf(stderr, "bind failed");
			exit(3);
		}

		//receive and send loop
		for (;;) {
		cliAddrLen = sizeof(echoClntAddr);

			if ((recvMsgSize = recvfrom(sock, &message, 
					sizeof(struct echo_msg),0, 
					(struct sockaddr *)&echoClntAddr, &cliAddrLen)) < 0) {
				fprintf(stderr, "recv failed");
				exit(4);
			}
			fprintf(stdout, "Handling client %s\n", 
					inet_ntoa(echoClntAddr.sin_addr));
			fprintf(stdout, "message = %s\n", message.msg);
			message.seq++;
			if (sendto(sock, &message, sizeof(struct echo_msg), 0,
					(struct sockaddr *) &echoClntAddr, 
					sizeof(echoClntAddr)) !=recvMsgSize) {
				fprintf(stderr, "sendto() failed");
				exit(5);
			}
		}
	}
}
