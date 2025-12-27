#include <stdio.h>
#include <time.h>
#include "server/history.h"


void log_match(const char *player1, const char *player2, const char *result) {
	FILE *fp = fopen("data/match_history.log", "a");
	if (!fp) return;


	time_t now = time(NULL);
	fprintf(fp, "%s | %s vs %s | %s\n",
			ctime(&now), player1, player2, result);


	fclose(fp);
}
