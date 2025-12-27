CC = gcc
CFLAGS = -Wall -pthread -Iinclude

SERVER_SRC = \
src/server/main.c \
src/server/server.c \
src/server/game.c \
src/server/matchmaker.c \
src/server/chat.c \
src/server/session.c \
src/server/history.c \
src/server/tournament.c \
src/common/utils.c

CLIENT_SRC = \
src/client/main.c \
src/client/client.c \
src/client/ui.c \
src/client/input.c \
src/common/utils.c

server:
	$(CC) $(CFLAGS) $(SERVER_SRC) -o server

client:
	$(CC) $(CFLAGS) $(CLIENT_SRC) -o client


