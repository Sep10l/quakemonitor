#ifndef OBJECT_H
#define OBJECT_H

#include "jsonParser.h"

typedef struct {
	JsonData data;
	MQTTClient Client;
} Object;

#endif