#include "object.h"
#include "jsonParser.h"
#include <stdio.h>
#include <string.h>
#include "MQTTClient.h"

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
	object->data.device_id = jsondata->device_id;
	object->data.last_seen = jsondata->last_seen;
	object->data.status = jsondata->status;
	object->data.uptime = jsondata->uptime;
}

void parse_json(const char* json_string, struct Object* object, cJSON* cjson)
{
	cjson = cJSON_Parse(json_string);
	if (cjson == NULL) {
		const char* error_ptr = cJSON_GetErrorPtr();
		if (error_ptr != NULL) {
			fprintf(stderr, "Error before: %s\n", error_ptr);
		}
		return;
	}

	cJSON* deviceid = cJSON_GetObjectItemCaseSensitive(cjson, "device_id");
	cJSON* status = cJSON_GetObjectItemCaseSensitive(cjson, "status");
	cJSON* uptime = cJSON_GetObjectItemCaseSensitive(cjson, "uptime");
	cJSON* lastseen = cJSON_GetObjectItemCaseSensitive(cjson, "last_seen");

	if (cJSON_IsString(deviceid) && (deviceid->valuestring != NULL) &&
		cJSON_IsString(status) && (status->valuestring != NULL) &&
		cJSON_IsNumber(uptime) &&
		cJSON_IsString(lastseen) && (lastseen->valuestring != NULL))
	{
		JsonData jsondata;
		memset(&jsondata, 0, sizeof(jsondata));
		jsondata.device_id = deviceid->valuestring;
		jsondata.status = status->valuestring;
		jsondata.uptime = uptime->valueint;
		jsondata.last_seen = lastseen->valuestring;

		setJsonData(&jsondata, object);
	}

	
}

void update_json_file(const char* json_string)
{
	FILE* fp = fopen("json.json", "w");
	if (fp == NULL) return;

	fputs(json_string, fp);
	fclose(fp);
}
