#ifndef OBJECT_H
#define OBJECT_H

#include "jsonParser.h"
#include <MQTTClient.h>

typedef struct JsonData JsonData;
typedef void* MQTTClient;

typedef struct Object {
	JsonData data;
	MQTTClient MClient;
	size_t size;
} Object;

void init_object(Object* object, size_t object_size);
size_t getObjectSize(Object* object);

#endif