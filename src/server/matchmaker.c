#include <pthread.h>
#include "game.h"


static int waiting_player = -1;
static pthread_mutex_t wait_lock = PTHREAD_MUTEX_INITIALIZER;
static game_t active_game;


int assign_player(int client_fd, game_t **game, int *player_idx) {
pthread_mutex_lock(&wait_lock);


if (waiting_player == -1) {
waiting_player = client_fd;
pthread_mutex_unlock(&wait_lock);
return 0; // waiting
}


init_game(&active_game, waiting_player, client_fd);
*game = &active_game;
*player_idx = (client_fd == active_game.player_fd[0]) ? 0 : 1;
waiting_player = -1;


pthread_mutex_unlock(&wait_lock);
return 1; // game started
}
