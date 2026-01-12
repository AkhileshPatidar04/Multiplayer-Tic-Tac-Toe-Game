
#include "server/chat.h"
#include "server/session.h"

void broadcast_chat(const message_t *msg, int sender_fd) {
	broadcast_session_chat( msg, sender_fd);
}



