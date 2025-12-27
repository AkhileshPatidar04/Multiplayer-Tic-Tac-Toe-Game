#ifndef PROTOCOL_H
#define PROTOCOL_H


typedef enum {
MSG_TEXT,
MSG_JOIN,
MSG_LEAVE
} message_type_t;


typedef struct {
message_type_t type;
char payload[256];
} message_t;


#endif
