#ifndef UTILS_H
#define UTILS_H
#include "protocol.h"

void die(const char *msg);

int recv_message(int sockfd, message_t *msg);

int send_message(int sockfd, const message_t *msg);
#endif
