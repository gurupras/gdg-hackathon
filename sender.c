/*
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,	25/11/94
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "Broadcast.h"

int main(int argc, char *argv[])
{
	int err;
	int res;
	char *message="Hello, World!";

	err = bcast_setup_tx_socket();
	if(err) {
		printf("Could not set up socket\n");
		return err;
	}

	/* now just sendto() our destination! */
	while (1) {
		res = bcast_tx(message, 0, strlen(message));
		if (res < 0) {
			perror("sendto");
			exit(1);
		}
		printf("Packet Sent\n");
		sleep(1);
	}
	return 0;
}
