#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "server/tournament.h"
#include "protocol.h"
#include "utils.h"
#include "DEBUG.h"


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



void print_leaderboard(int client_fd)
{
    message_t msg;

    // Header line
    memset(&msg, 0, sizeof(msg));
    msg.type = MSG_GAME_STATE;   // or MSG_LEADERBOARD if you add one
    snprintf(msg.payload, sizeof(msg.payload), "--- Leaderboard ---");
    send_message(client_fd, &msg);

    // Each player entry
    for (int i = 0; i < player_count; i++) {
        memset(&msg, 0, sizeof(msg));
        msg.type = MSG_GAME_STATE;

        snprintf(msg.payload, sizeof(msg.payload),
                 "%s | W:%d L:%d D:%d",
                 players[i].player_name,
                 players[i].wins,
                 players[i].losses,
                 players[i].draws);

        send_message(client_fd, &msg);
    }
}

