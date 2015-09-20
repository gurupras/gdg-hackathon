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

#include "Broadcast.h"

#define BCAST_PORT 1234
#define BCAST_GROUP "225.0.0.37"

static struct sockaddr_in bcast_addr;
static int bcast_fd = -1;

int bcast_setup_socket()
{
    if (bcast_fd > 0)
    {
        return 0;
    }

     /* create what looks like an ordinary UDP socket */
    if ((bcast_fd = socket(AF_INET,SOCK_DGRAM,0)) < 0)
    {
        return -1;
    }

    /* set up destination address */
    memset(&bcast_addr, 0, sizeof(bcast_addr));
    bcast_addr.sin_family=AF_INET;
    bcast_addr.sin_addr.s_addr=inet_addr(BCAST_GROUP);
    bcast_addr.sin_port=htons(BCAST_PORT);

    return 0;
}

int bcast_send(void *buffer, int start, int len)
{
    return sendto(bcast_fd,buffer + start, len, 0,(struct sockaddr *) &bcast_addr, sizeof(bcast_addr));
}

int bcast_tear_down()
{
    close(bcast_fd);
    bcast_fd = -1;
    return 0;
}

