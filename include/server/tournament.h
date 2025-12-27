#ifndef TOURNAMENT_H
#define TOURNAMENT_H


#define MAX_PLAYERS 100


typedef struct {
	char player_name[32];
	int wins;
	int losses;
	int draws;
} player_stats_t;


void record_result(const char *player1, const char *player2, int result);
void save_leaderboard();
void load_leaderboard();
void print_leaderboard(int client_fd);


#endif
