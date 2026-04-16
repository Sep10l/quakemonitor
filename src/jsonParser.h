#include <stdlib.h>
#include "../res/cJSON.h"

#include "MQTTClient.h"

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

typedef struct {
	JsonData data;
	MQTTClient MQTTClient;
} Object;

void setJsonData(JsonData* jsondata, Object* object);
void parse_json(const char* json_string, Object* object, cJSON* json);