#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#include "common.h"
#include "utils.h"


void start_client(const char* server_ip) {
	int sock;
	struct sockaddr_in server_addr;
	char buffer[BUFFER_SIZE];


	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock < 0) die("socket");


	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SERVER_PORT);
	inet_pton(AF_INET, server_ip, &server_addr.sin_addr);


	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
		die("connect");


	printf("Connected to server. Type messages:\n");


	while (fgets(buffer, sizeof(buffer), stdin)) {
		send(sock, buffer, strlen(buffer), 0);
		int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
		if (bytes <= 0) break;
		buffer[bytes] = '\0';
		printf("Server: %s", buffer);
	}


	close(sock);
}
