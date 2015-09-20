/*
 * Some of this was borrowed from:
 *
 * sender.c -- multicasts "hello, world!" to a multicast group once a second
 *
 * Antony Courtney,     25/11/94
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
#include <errno.h>

#include "Broadcast.h"

#define BCAST_PORT 1234
#define BCAST_GROUP "225.0.0.37"

static struct sockaddr_in bcast_tx_addr, bcast_rx_addr;
static int bcast_tx_fd = -1, bcast_rx_fd = -1;

int bcast_setup_tx_socket()
{
	if (bcast_tx_fd > 0)
	{
		return 0;
	}

	/* create what looks like an ordinary UDP socket */
	if ((bcast_tx_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
	{
		return -1;
	}

	/* set up destination address */
	memset(&bcast_tx_addr, 0, sizeof(bcast_tx_addr));
	bcast_tx_addr.sin_family=AF_INET;
	bcast_tx_addr.sin_addr.s_addr=inet_addr(BCAST_GROUP);
	bcast_tx_addr.sin_port=htons(BCAST_PORT);

	return 0;
}

int bcast_setup_rx_socket()
{
	int yes;
	struct ip_mreq mreq;
	/* create what looks like an ordinary UDP socket */
	if ((bcast_rx_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}


	/**** MODIFICATION TO ORIGINAL */
	/* allow multiple sockets to use the same PORT number */
	if (setsockopt(bcast_rx_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		perror("Reusing ADDR failed");
		exit(1);
	}
	/*** END OF MODIFICATION TO ORIGINAL */

	/* set up destination address */
	memset(&bcast_rx_addr, 0, sizeof(bcast_rx_addr));
	bcast_rx_addr.sin_family = AF_INET;
	bcast_rx_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* N.B.: differs from sender */
	bcast_rx_addr.sin_port = htons(BCAST_PORT);

	/* bind to receive address */
	if (bind(bcast_rx_fd, (struct sockaddr *) &bcast_rx_addr, sizeof(bcast_rx_addr)) < 0) {
		perror("bind");
		exit(1);
	}

	/* use setsockopt() to request that the kernel join a multicast group */
	mreq.imr_multiaddr.s_addr = inet_addr(BCAST_GROUP);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(bcast_rx_fd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq)) < 0) {
		perror("setsockopt");
		exit(1);
	}
	return 0;
}

int bcast_tx(void *buffer, int start, int len)
{
	return sendto(bcast_tx_fd, buffer + start,
			len, 0, (struct sockaddr *) &bcast_tx_addr,
			sizeof(bcast_tx_addr));
}

int bcast_rx(void *buffer, int len)
{
	unsigned int addrlen = sizeof(bcast_rx_addr);
	return recvfrom(bcast_rx_fd, buffer, len, 0,
				(struct sockaddr *) &bcast_rx_addr,
				&addrlen);
}

int bcast_tear_down_tx()
{
	close(bcast_tx_fd);
	bcast_tx_fd = -1;
	return 0;
}

int bcast_tear_down_rx()
{
	close(bcast_rx_fd);
	bcast_rx_fd = -1;
	return 0;
}

