#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server/tournament.h"

static player_stats_t players[MAX_PLAYERS];
static int player_count = 0;


static player_stats_t* get_player(const char *name) {
	for (int i = 0; i < player_count; i++)
		if (strcmp(players[i].player_name, name) == 0)
			return &players[i];


	strcpy(players[player_count].player_name, name);
	players[player_count].wins = 0;
	players[player_count].losses = 0;
	players[player_count].draws = 0;
	return &players[player_count++];
}


void record_result(const char *p1, const char *p2, int result) {
	player_stats_t *a = get_player(p1);
	player_stats_t *b = get_player(p2);


	if (result == 1) {
		a->wins++; b->losses++;
	} else if (result == -1) {
		b->wins++; a->losses++;
	} else {
		a->draws++; b->draws++;
	}
}


void save_leaderboard() {
	FILE *fp = fopen("data/leaderboard.db", "w");
	if (!fp) return;


	for (int i = 0; i < player_count; i++) {
		fprintf(fp, "%s %d %d %d\n",
				players[i].player_name,
				players[i].wins,
				players[i].losses,
				players[i].draws);
	}
	fclose(fp);
}


void load_leaderboard() {
	FILE *fp = fopen("data/leaderboard.db", "r");
	if (!fp) return;


	while (fscanf(fp, "%s %d %d %d",
				players[player_count].player_name,
				&players[player_count].wins,
				&players[player_count].losses,
				&players[player_count].draws) != EOF) {
		player_count++;
	}
	fclose(fp);
}


void print_leaderboard(int client_fd) {
	char buffer[256];
	send(client_fd, "--- Leaderboard ---\n", 20, 0);
	for (int i = 0; i < player_count; i++) {
		snprintf(buffer, sizeof(buffer), "%s | W:%d L:%d D:%d\n",
				players[i].player_name,
				players[i].wins,
				players[i].losses,
				players[i].draws);
		send(client_fd, buffer, strlen(buffer), 0);
	}
}
