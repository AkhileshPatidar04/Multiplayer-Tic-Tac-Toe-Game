#ifndef PROTOCOL_H
#define PROTOCOL_H


typedef enum {
    MSG_HELLO = 1,          // client -> server (connect)
    MSG_WELCOME,            // server -> client (player_id assigned)

    MSG_MATCH_REQUEST,      // client -> server
    MSG_MATCH_FOUND,        // server -> client

    MSG_MOVE,               // client -> server
    MSG_GAME_STATE,         // server -> client

    MSG_GAME_OVER,          // server -> client
    
    MSG_CHAT,               // bidirectional

    MSG_ERROR,               // server -> client
    MSG_LEAVE               // client ->server
} message_type_t;


typedef struct {
    message_type_t type;
    char payload[256];
} message_t;



#endif