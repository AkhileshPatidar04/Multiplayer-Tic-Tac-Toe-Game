#ifndef GAME_H
#define GAME_H


#include <pthread.h>


#define BOARD_SIZE 3


typedef struct {
    char board[BOARD_SIZE][BOARD_SIZE];
    int player_fd[2];
    int current_turn; // 0 or 1
    int active;
    pthread_mutex_t lock;
} game_t;


void init_game(game_t *game, int p1, int p2);
int make_move(game_t *game, int player_fd, int row, int col);
int check_winner(game_t *game);
int is_draw(game_t *game);
void reset_game(game_t *game);
int opponent(game_t *game, int client_fd);


#endif
