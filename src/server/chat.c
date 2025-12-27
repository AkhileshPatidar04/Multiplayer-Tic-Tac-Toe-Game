#include <string.h>
#include <unistd.h>
#include "server/chat.h"


extern int client_fds[50];
extern int client_count;


void broadcast_chat(const char *msg, int sender_fd) {
	for (int i = 0; i < client_count; i++) {
		if (client_fds[i] != sender_fd) {
			send(client_fds[i], msg, strlen(msg), 0);
		}
	}
}
