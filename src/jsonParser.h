#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <stdlib.h>
#include "object.h"
#include "MQTTClient.h"
#include "../res/cJSON.h"

typedef struct {
	char* device_id;
	char* status;
	int uptime;
	char* last_seen;
} JsonData;

const char* GetDeviceID();
const char* GetDeviceStatus();
const int* GetUptime();
const char* GetLastLogin();

char* load_file(const char* filename);

void setJsonData(JsonData* jsondata, Object* object);
void parse_json(const char* json_string, Object* object, cJSON* json);
void update_json_file(const char* json_string);

#endif