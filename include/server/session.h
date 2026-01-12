#ifndef SESSION_H
#define SESSION_H

#include "protocol.h"
#include "utils.h"
#define SESSION_ID_LEN 32


typedef struct {
	int client_fd;
	char session_id[SESSION_ID_LEN];
	char player_name[32];
} session_t;


void create_session(int client_fd, const char *player_name);
void remove_session(int client_fd);
const char* get_session_id(int client_fd);
void broadcast_session_chat(const message_t *msg, int sender_fd);

#endif
