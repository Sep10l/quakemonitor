#include "jsonParser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../res/cJSON.h"

const char* GetDeviceID()
{
	return NULL;
}

const char* GetDeviceStatus()
{
	return NULL;
}

const int* GetUptime()
{
	return NULL;
}

const char* GetLastLogin()
{
	return NULL;
}

char* load_file(const char* filename)
{
	FILE* f = fopen(filename, "rb");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	long length = ftell(f);
	fseek(f, 0, SEEK_SET);

	char* buffer = malloc(length + 1);
	if (buffer) {
		fread(buffer, 1, length, f);
		buffer[length] = '\0';
	}
	fclose(f);
	return buffer;
}

void setJsonData(JsonData* jsondata, Object* object) 
{
	object->data = *jsondata;
}

void parse_json(const char* json_string, Object* object, cJSON* json)
{
	json = cJSON_Parse(json_string);
	if (json == NULL) {
		const char* error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		return;
	}

	cJSON* deviceid = cJSON_GetObjectItemCaseSensitive(json, "device_id");
	cJSON* status = cJSON_GetObjectItemCaseSensitive(json, "status");
	cJSON* uptime = cJSON_GetObjectItemCaseSensitive(json, "uptime");
	cJSON* lastseen = cJSON_GetObjectItemCaseSensitive(json, "last_seen");

	if (cJSON_IsString(deviceid) && (deviceid->valuestring != NULL) &&
		cJSON_IsString(status) && (status->valuestring != NULL) &&
		cJSON_IsNumber(uptime) &&
		cJSON_IsString(lastseen) && (lastseen->valuestring != NULL))
	{
		JsonData jsondata;
		jsondata.device_id = deviceid->valuestring;
		jsondata.status = status->valuestring;
		jsondata.uptime = uptime->valueint;
		jsondata.last_seen = lastseen->valuestring;

		setJsonData(&jsondata, object);
	}

	
}
