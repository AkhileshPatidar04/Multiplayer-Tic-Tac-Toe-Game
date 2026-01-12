#ifndef MATCHMAKER_H
#define MATCHMAKER_H

#include "game.h"

int assign_player(int client_fd, game_t **game, int *player_idx);
game_t* wait_for_game(int client_fd, int *player_idx);

#endif
