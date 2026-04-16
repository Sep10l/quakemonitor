#pragma once
#include <stdbool.h>
#include "jsonParser.h"

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
