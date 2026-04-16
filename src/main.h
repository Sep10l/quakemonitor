#ifndef MAIN_H
#define MAIN_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <stdbool.h>
#include "jsonParser.h"
#include <winsock2.h>
#include "MQTTClient.h"
#include "object.h"

typedef int socklen_t;
const char* localhost = "127.0.0.1";


typedef struct {
	const char* serverURI;
	const char* clientID;
} MQTTInitOptions;

JsonData* checkValidJSON(JsonData* jsondata);
void create_object(Object* object, JsonData* jsondata);
bool printJSON(Object* object);
int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message);
void init_mqtt(Object* object, MQTTInitOptions* init_options, MQTTClient_connectOptions* connect_options, MQTTClient_SSLOptions* ssl_options);
void setup_openssl();

void bind_socket(SOCKET* server_socket);

char* http_request(const char* host, const char* path);

#endif