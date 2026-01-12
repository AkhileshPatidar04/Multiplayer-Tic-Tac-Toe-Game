#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>
#include "utils.h"
#include "DEBUG.h"

void die(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int recv_message(int sockfd, message_t *msg)
{
    size_t total = 0;
    size_t len = sizeof(message_t);
    char *data = (char *)msg;

    while (total < len) {
        ssize_t recvd = recv(sockfd, data + total, len - total, 0);
        if (recvd <= 0)
        return -1;
        total += recvd;
    }

    #ifdef DEBUG
        printf("[DEBUG] recv: type=%d payload='%s'\n",
       msg->type, msg->payload);
    #endif 


    return 0;
}
int send_message(int sockfd, const message_t *msg)
{
    size_t total = 0;
    size_t len = sizeof(message_t);
    const char *data = (const char *)msg;

    #ifdef DEBUG
        printf("[DEBUG] send: type=%d payload='%s'\n",
       msg->type, msg->payload);
    #endif

    while (total < len) {
        ssize_t sent = send(sockfd, data + total, len - total, 0);
        if (sent <= 0)
            return -1;
        total += sent;
    }

    return 0;
}
