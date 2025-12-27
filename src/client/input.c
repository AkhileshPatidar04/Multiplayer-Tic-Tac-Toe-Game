#include <stdio.h>
#include "client/input.h"


void read_move(int *row, int *col) {
	printf("Enter move (row col): ");
	scanf("%d %d", row, col);
}


void read_chat(char *buffer) {
	printf("[chat] ");
	fgets(buffer, 256, stdin);
}
