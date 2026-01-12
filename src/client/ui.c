#include <stdio.h>
#include "client/ui.h"

// extern int board[3][3];
void draw_board(char (*board)[3]) {
	printf("\n");
	for (int i = 0; i < 3; i++) {
		printf(" %c | %c | %c \n", board[i][0], board[i][1], board[i][2]);
		if (i < 2) printf("---+---+---\n");
	}
	printf("\n");
}


void show_message(const char *msg) {
	printf("%s\n", msg);
}
