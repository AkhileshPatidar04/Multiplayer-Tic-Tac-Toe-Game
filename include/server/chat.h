#ifndef CHAT_H
#define CHAT_H

#include "protocol.h"

void broadcast_chat(const message_t *msg, int sender_fd);


#endif
