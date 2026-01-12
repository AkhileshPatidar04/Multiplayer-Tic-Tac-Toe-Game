#include<stdio.h>
#include <pthread.h>
#include <unistd.h>

#include "DEBUG.h"
#include "server/matchmaker.h"

static int waiting_player = -1;
static pthread_mutex_t wait_lock = PTHREAD_MUTEX_INITIALIZER;
static game_t active_game;

int assign_player(int client_fd, game_t **game, int *player_idx) {
    
    #ifdef DEBUG
        printf("[DEBUG]:assigning player\n");
    #endif


    pthread_mutex_lock(&wait_lock);

    if (waiting_player == -1) {
        waiting_player = client_fd;
        pthread_mutex_unlock(&wait_lock);
        
        
        #ifdef DEBUG
            printf("[DEBUG]:wait for player\n");
        #endif


        return 0; // waiting
    }


    init_game(&active_game, waiting_player, client_fd);
    *game = &active_game;
    *player_idx = (client_fd == active_game.player_fd[0]) ? 0 : 1;
    waiting_player = -1;

    
    pthread_mutex_unlock(&wait_lock);
    
    #ifdef DEBUG
        printf("[DEBUG] : new game started with Client %d and Client %d\n",waiting_player, client_fd);
    #endif
    return 1; // game started
}
game_t* wait_for_game(int client_fd, int *player_idx)
{
    while (1) {
        pthread_mutex_lock(&wait_lock);

        if (active_game.active &&
           (client_fd == active_game.player_fd[0] ||
            client_fd == active_game.player_fd[1])) {

            *player_idx = (client_fd == active_game.player_fd[0]) ? 0 : 1;
            pthread_mutex_unlock(&wait_lock);
            return &active_game;
        }

        pthread_mutex_unlock(&wait_lock);
        usleep(100000);
    }
}