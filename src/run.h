#ifndef MAIN_H
#define MAIN_H

#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "object.h"
#include "MQTTClient.h"
#include <stdbool.h>
#include "jsonParser.h"
#include <curl/curl.h>

#ifdef __cplusplus
extern "C" {
#endif

//typedef struct Object Object;
typedef void* MQTTClient;

typedef struct MQTTInitOptions {
	const char* serverURI;
	const char* clientID;
} MQTTInitOptions;

typedef struct Array {
	void* data;
	int size;
	int capacity;
	size_t elem_size;
} Array;

void* get(Array* a, int index);
void push_back(Array* a, void* element);
void init_array(Array* a, size_t elem_size);
Object* getObject();

JsonData* checkValidJSON(JsonData* jsondata);
void create_object(Object* object, JsonData* jsondata);
bool printJSON(Object* object);
int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message);
void init_mqtt(Object* object, MQTTInitOptions* init_options, MQTTClient_connectOptions* connect_options, MQTTClient_SSLOptions* ssl_options);
void setup_openssl();

void bind_socket(SOCKET* server_socket);

int init_winsock();
size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream);
void init_curl(CURL* curl);
void cleanup_curl(CURL* curl);

CURLcode HTTP_get_request(CURL* curl, const char* url);
CURLcode HTTP_post_request(CURL* curl, const char* url, const char* body);
CURLcode HTTP_put_request(CURL* curl, const char* url, const char* body);



int quakemonitor_run();

int quakemonitor_update();
void quakemonitor_cleanup();

int run_app();

static struct Object* object;
static cJSON* json = NULL;
static CURL* curl = NULL;

#ifdef __cplusplus
}
#endif

#endif