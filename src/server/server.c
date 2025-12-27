#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>


#include "common.h"
#include "protocol.h"
#include "utils.h"

int client_fds[MAX_CLIENTS];
int client_count = 0;

void* handle_client(void* arg);


void start_server() {
	int server_fd, client_fd;
	struct sockaddr_in server_addr, client_addr;
	socklen_t addr_len = sizeof(client_addr);


	server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0) die("socket");


	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(SERVER_PORT);


	if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		die("bind");


	if (listen(server_fd, MAX_CLIENTS) < 0)
		die("listen");


	printf("[SERVER] Listening on port %d...\n", SERVER_PORT);


	while (1) {
		client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
		if (client_fd < 0) continue;


		pthread_t tid;
		int* pclient = malloc(sizeof(int));
		*pclient = client_fd;


		pthread_create(&tid, NULL, handle_client, pclient);
		pthread_detach(tid);
	}
}


void* handle_client(void* arg) {
	int client_fd = *((int*)arg);
	free(arg);


	char buffer[BUFFER_SIZE];
	int bytes;


	printf("[SERVER] Client connected: %d\n", client_fd);


	while ((bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
		buffer[bytes] = '\0';
		printf("[CLIENT %d]: %s", client_fd, buffer);
		send(client_fd, buffer, strlen(buffer), 0); // echo
	}


	printf("[SERVER] Client disconnected: %d\n", client_fd);
	close(client_fd);
	return NULL;
}
