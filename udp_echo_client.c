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
	struct sockaddr_in fromAddr;
	unsigned short echoServerPort;
	socklen_t fromSize;
	char *servIP;
	int echoMsgLen;
	int respMsgLen;
	struct msg_echo message;

	if ((argc < 3) || (argc >4 )) {
		fprintf(stderr, "Usage: %s <Server IP><Echo Word>\n", argv[0]);
		exit(1);
	}

	servIP = argv[1];
	strcpy(message.msg, argv[2]);
	
	if (argc == 4) {
		echoServerPort = atoi(argv[3]);
	} else {
		echoServerPort = 7;
	}


	//create a socket
	if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
		fprintf(stderr, "sock failed\n");
		exit(2);
	}
	//initialize
	memset(&echoServAddr, 0, sizeof(echoServAddr));
	echoServAddr.sin_family = AF_INET;
	echoServAddr.sin_addr.s_addr = inet_addr(servIP);
	echoServAddr.sin_port = htons(echoServerPort);

	if (sendto(sock, &message, sizeof(struct msg_echo), 0, 
	(struct sockaddr *)&echoServAddr, sizeof(echoServAddr)) < 0) {
		fprintf(stderr, "sendto failed\n");
		exit(4);
	}

	fromSize = sizeof(fromAddr);
	if ((respMsgLen = recvfrom(sock, &message, sizeof(struct msg_echo), 0,
			(struct sockaddr *)&echoServAddr, &fromSize)) < 0 ) {
	   	fprintf(stderr, "recv failed\n");
		exit(5);
	}
	fprintf(stdout, "message = %s\n", message.msg);
	close(sock);
	exit(0);	
}
