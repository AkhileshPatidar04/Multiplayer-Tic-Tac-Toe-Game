#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "server/session.h"


#define MAX_SESSIONS 100


static session_t sessions[MAX_SESSIONS];
static int session_count = 0;


static void generate_session_id(char *buf) {
	srand(time(NULL) ^ rand());
	snprintf(buf, SESSION_ID_LEN, "%ld-%d", time(NULL), rand());
}


void create_session(session_t *s, int client_fd, const char *player_name) {
	s->client_fd = client_fd;
	strncpy(s->player_name, player_name, sizeof(s->player_name));
	generate_session_id(s->session_id);


	sessions[session_count++] = *s;
}


void remove_session(int client_fd) {
	for (int i = 0; i < session_count; i++) {
		if (sessions[i].client_fd == client_fd) {
			sessions[i] = sessions[--session_count];
			return;
		}
	}
}


const char* get_session_id(int client_fd) {
	for (int i = 0; i < session_count; i++) {
		if (sessions[i].client_fd == client_fd)
			return sessions[i].session_id;
	}
	return NULL;
}
