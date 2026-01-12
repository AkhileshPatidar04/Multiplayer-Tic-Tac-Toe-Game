// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "common.h"
#include "utils.h"
#include "protocol.h"
#include "client/ui.h"
#include "client/input.h"

static char board[3][3];
static pthread_mutex_t board_lock = PTHREAD_MUTEX_INITIALIZER;
static int my_player = -1;

/* ================= SERVER LISTENER ================= */

void *listen_server(void *arg)
{
    int sock = *((int *)arg);
    free(arg);

    message_t msg;

    while (1) {
        if (recv_message(sock, &msg) < 0) {
            show_message("[CLIENT] Server disconnected");
            exit(0);
        }

        switch (msg.type) {

        case MSG_WELCOME:
            show_message(msg.payload);
            break;

        case MSG_MATCH_REQUEST:
            show_message(msg.payload);
            break;

        case MSG_MATCH_FOUND:
            show_message(msg.payload);
            sscanf(msg.payload, "Match found. You are Player %d", &my_player);
            my_player--; // make 0-based
            break;

        case MSG_GAME_STATE: {
            int r, c, p;
            if (sscanf(msg.payload, "MOVE %d %d by Player %d", &r, &c, &p) == 3) {
                pthread_mutex_lock(&board_lock);
                board[r][c] = (p == 1) ? 'X' : 'O';
                draw_board(board);
                pthread_mutex_unlock(&board_lock);
            }
            break;
        }

        case MSG_GAME_OVER:
            show_message(msg.payload);
            break;

        case MSG_CHAT:
            printf("[CHAT] %s\n", msg.payload);
            break;

        case MSG_ERROR:
            printf("[ERROR] %s\n", msg.payload);
            break;

        default:
            show_message("[CLIENT] Unknown message");
        }
    }
    return NULL;
}

/* ================= CLIENT START ================= */

void start_client(const char *server_ip)
{
    int sock;
    struct sockaddr_in server_addr;
    message_t msg;

    memset(board, 0, sizeof(board));

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
        die("socket");

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, server_ip, &server_addr.sin_addr);

    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        die("connect");

    show_message("Connected to server");

    /* ========== HELLO ========== */
    msg.type = MSG_HELLO;
    snprintf(msg.payload, sizeof(msg.payload), "Player");
    send_message(sock, &msg);

    /* ========== LISTENER THREAD ========== */
    pthread_t tid;
    int *arg = malloc(sizeof(int));
    *arg = sock;
    pthread_create(&tid, NULL, listen_server, arg);
    pthread_detach(tid);

    /* ========== INPUT LOOP ========== */
    while (1) {
        char cmd[32];
        printf("\nCommands: move | chat | quit\n> ");
        fgets(cmd, sizeof(cmd), stdin);

        if (strncmp(cmd, "move", 4) == 0) {
            int r, c;
            read_move(&r, &c);

            msg.type = MSG_MOVE;
            snprintf(msg.payload, sizeof(msg.payload), "%d %d", r, c);
            send_message(sock, &msg);
        }
        else if (strncmp(cmd, "chat", 4) == 0) {
            char buffer[256];
            read_chat(buffer);

            msg.type = MSG_CHAT;
            strncpy(msg.payload, buffer, sizeof(msg.payload) - 1);
            send_message(sock, &msg);
        }
        else if (strncmp(cmd, "quit", 4) == 0) {
            msg.type = MSG_LEAVE;
            strcpy(msg.payload, "Leaving");
            send_message(sock, &msg);
            break;
        }
        else {
            show_message("Unknown command");
        }
    }

    close(sock);
}
