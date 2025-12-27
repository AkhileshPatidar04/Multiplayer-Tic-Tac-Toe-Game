#ifndef SESSION_H
#define SESSION_H


#define SESSION_ID_LEN 32


typedef struct {
	int client_fd;
	char session_id[SESSION_ID_LEN];
	char player_name[32];
} session_t;


void create_session(session_t *s, int client_fd, const char *player_name);
void remove_session(int client_fd);
const char* get_session_id(int client_fd);


#endif
