# =========================
# Compiler and flags
# =========================
CC      = gcc
CFLAGS  = -Wall -Wextra -g -pthread -Iinclude
LDFLAGS = -pthread

# =========================
# Output binaries
# =========================
SERVER_BIN = server
CLIENT_BIN = client

# =========================
# Source files
# =========================
COMMON_SRC = src/common/utils.c

SERVER_SRC = \
	src/server/main.c \
	src/server/server.c \
	src/server/game.c \
	src/server/chat.c \
	src/server/history.c \
	src/server/matchmaker.c \
	src/server/session.c \
	src/server/tournament.c \
	$(COMMON_SRC)

CLIENT_SRC = \
	src/client/main.c \
	src/client/client.c \
	src/client/input.c \
	src/client/ui.c \
	$(COMMON_SRC)

# =========================
# Object files
# =========================
SERVER_OBJ = $(SERVER_SRC:.c=.o)
CLIENT_OBJ = $(CLIENT_SRC:.c=.o)

# =========================
# Targets
# =========================
.PHONY: all clean server client

all: server client

server: $(SERVER_OBJ)
	$(CC) $(SERVER_OBJ) -o $(SERVER_BIN) $(LDFLAGS)

client: $(CLIENT_OBJ)
	$(CC) $(CLIENT_OBJ) -o $(CLIENT_BIN) $(LDFLAGS)

# =========================
# Pattern rule
# =========================
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# =========================
# Cleanup
# =========================
clean:
	rm -f $(SERVER_OBJ) $(CLIENT_OBJ) $(SERVER_BIN) $(CLIENT_BIN)
