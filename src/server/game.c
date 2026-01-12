#include <string.h>
#include "game.h"


void init_game(game_t *game, int p1, int p2) {
	memset(game->board, ' ', sizeof(game->board));
	game->player_fd[0] = p1;
	game->player_fd[1] = p2;
	game->current_turn = 0;
	game->active = 1;
	pthread_mutex_init(&game->lock, NULL);
}


int make_move(game_t *game, int player_fd, int row, int col) {
	pthread_mutex_lock(&game->lock);


	int idx = (player_fd == game->player_fd[0]) ? 0 : 1;
	if (idx != game->current_turn) {
		pthread_mutex_unlock(&game->lock);
		return -1; // not your turn
	}


	if (row < 0 || row >= 3 || col < 0 || col >= 3 || game->board[row][col] != ' ') {
		pthread_mutex_unlock(&game->lock);
		return -2; // invalid move
	}


	game->board[row][col] = (idx == 0) ? 'X' : 'O';
	game->current_turn = 1 - game->current_turn;


	pthread_mutex_unlock(&game->lock);
	return 0;
}


int check_winner(game_t *game) {
	char (*b)[3] = game->board;


	for (int i = 0; i < 3; i++) {
		if (b[i][0] != ' ' && b[i][0] == b[i][1] && b[i][1] == b[i][2]) return 1;
		if (b[0][i] != ' ' && b[0][i] == b[1][i] && b[1][i] == b[2][i]) return 1;
	}


	if (b[0][0] != ' ' && b[0][0] == b[1][1] && b[1][1] == b[2][2]) return 1;
	if (b[0][2] != ' ' && b[0][2] == b[1][1] && b[1][1] == b[2][0]) return 1;


	return -1;
}


int is_draw(game_t *game) {
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			if (game->board[i][j] == ' ')
				return 0;
	return 1;
}


void reset_game(game_t *game) {
	memset(game->board, ' ', sizeof(game->board));
	game->current_turn = 0;
}



int opponent(game_t *game, int client_fd)
{
	return (game->player_fd[0] == client_fd) ? game->player_fd[1] : game->player_fd[0]; 
}