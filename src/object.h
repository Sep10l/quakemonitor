#ifndef OBJECT_H
#define OBJECT_H

#include "jsonParser.h"
#include <MQTTClient.h>

typedef struct JsonData JsonData;
typedef void* MQTTClient;

typedef struct Object {
	JsonData data;
	MQTTClient MClient;
} Object;

#endif