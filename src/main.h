#pragma once
#include <stdbool.h>
#include "jsonParser.h"
#include <winsock2.h>

typedef int socklen_t;

typedef struct {
	JsonData data;
	MQTTClient MQTTClient;
} Object;

typedef struct {
	const char* serverURI;
	const char* clientID;
} MQTTInitOptions;

typedef struct {
	char* data;
	int size;
} Buffer;

typedef struct {
	unsigned char* data;
	int size;
} uBuffer;


JsonData* checkValidJSON(JsonData* jsondata);
void create_object(Object* object, JsonData* jsondata);
bool printJSON(Object* object);
int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message);
void init_mqtt(Object* object, MQTTInitOptions* init_options, MQTTClient_connectOptions* connect_options, MQTTClient_SSLOptions* ssl_options);
void setup_openssl();

void bind_socket(SOCKET* server_socket);
uBuffer* allocate_ubuffer(int size);

char* http_request(const char* host, const char* path);
