#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#include "common.h"
#include "protocol.h"
#include "utils.h"
#include "DEBUG.h"
#include "game.h"
#include "server/matchmaker.h"
#include "server/session.h"
#include "server/chat.h"

// int client_fds[MAX_CLIENTS];
// int client_count = 0;

void* handle_client(void* arg);


void start_server() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);


	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) die("socket");


	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);


	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		die("bind");


	if (listen(server_fd, MAX_CLIENTS) < 0)
		die("listen");


	printf("[SERVER] Listening on port %d...\n", SERVER_PORT);


	while (1) {
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
		if (client_fd < 0) continue;


		pthread_t tid;
		int* pclient = malloc(sizeof(int));
		*pclient = client_fd;
    

		pthread_create(&tid, NULL, handle_client, pclient);
		pthread_detach(tid);
	}
}


void* handle_client(void* arg)
{
    int client_fd = *((int*)arg);
    free(arg);

    message_t msg;
    game_t *game = NULL;
    int player_idx = -1;
    int status = 0;
    char player_name[32] = {0};
    

    printf("[SERVER] Client connected: fd=%d\n", client_fd);

    /* ================= HANDSHAKE ================= */
    if (recv_message(client_fd, &msg) < 0 || msg.type != MSG_HELLO) {
        close(client_fd);
        return NULL;
    }

    #ifdef DEBUG
        printf("[DEBUG]: HANDSHAKE DONE\n");
    #endif

    strncpy(player_name, msg.payload, sizeof(player_name) - 1);
    
    create_session(client_fd, player_name);
    
    #ifdef DEBUG
        printf("[DEBUG]: new session created DONE\n");
    #endif

    msg.type = MSG_WELCOME;
    snprintf(msg.payload, sizeof(msg.payload),
             "Welcome %s", player_name);
    send_message(client_fd, &msg);

    /* ================= MATCHMAKING ================= */
   	status = assign_player(client_fd, &game, &player_idx);

    if (status == 0) {
        msg.type = MSG_MATCH_REQUEST;
        strcpy(msg.payload, "Waiting for opponent...");
        send_message(client_fd, &msg);

        game = wait_for_game(client_fd, &player_idx);
    }

    msg.type = MSG_MATCH_FOUND;
    snprintf(msg.payload, sizeof(msg.payload),
            "Match found. You are Player %d", player_idx + 1);
    send_message(client_fd, &msg);



    /* ================= GAME LOOP ================= */
    while (1) {
        if (recv_message(client_fd, &msg) < 0)
            break;

        switch (msg.type) {

        case MSG_MOVE: {
                    int row, col;
                    sscanf(msg.payload, "%d %d", &row, &col);

                    int res = make_move(game, client_fd, row, col);

                    if (res < 0) {
                        msg.type = MSG_ERROR;
                        strcpy(msg.payload, "Invalid move");
                        send_message(client_fd, &msg);
                        break;
                    }

                    msg.type = MSG_GAME_STATE;
                    snprintf(msg.payload, sizeof(msg.payload),
                            "MOVE %d %d by Player %d",
                            row, col, player_idx + 1);
                    send_message(client_fd, &msg);
                    send_message(opponent(game, client_fd), &msg);

                    int winner = check_winner(game);
                    if (winner >= 0) {
                        msg.type = MSG_GAME_OVER;
                        snprintf(msg.payload, sizeof(msg.payload),
                                "Player %d wins", winner + 1);
                        send_message(client_fd, &msg);
                        send_message(opponent(game, client_fd), &msg);
                        break;
                    }

                    if (is_draw(game)) {
                        msg.type = MSG_GAME_OVER;
                        strcpy(msg.payload, "Draw");
                        send_message(client_fd, &msg);
                        break;
                    }
                    break;
                }

        case MSG_CHAT:
            broadcast_chat(&msg, client_fd);
            break;

        case MSG_LEAVE:
            goto disconnect;

        default:
            msg.type = MSG_ERROR;
            strcpy(msg.payload, "Unknown message");
            send_message(client_fd, &msg);
        }
    }

disconnect:
    printf("[SERVER] Client disconnected: fd=%d\n", client_fd);
    remove_session(client_fd);
    close(client_fd);
    return NULL;
}
