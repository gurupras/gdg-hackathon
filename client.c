/*
 * listener.c -- joins a multicast group and echoes all data it receives from
 *		the group to its stdout...
 *
 * Antony Courtney,	25/11/94
 * Modified by: Frédéric Bastien (25/03/04)
 * to compile without warning and work correctly
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "Broadcast.h"

#define MSGBUFSIZE 256

int main(int argc, char *argv[])
{
	int nbytes;
	char msgbuf[MSGBUFSIZE];
	bcast_setup_rx_socket();
	/* now just enter a read-print loop */
	while (1) {
		bzero(msgbuf, MSGBUFSIZE);
		if ((nbytes = bcast_rx(msgbuf,MSGBUFSIZE) < 0)) {
			perror("recvfrom");
			exit(1);
		}
		printf("%s\n",msgbuf);
	}
}
