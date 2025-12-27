#include <stdio.h>
#include <stdlib.h>
#include "utils.h"


void die(const char *msg) {
perror(msg);
exit(EXIT_FAILURE);
}
