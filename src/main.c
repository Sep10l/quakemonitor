#pragma comment(lib, "Ws2_32.lib")
#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <conio.h>

#include "../res/cJSON/cJSON.h"

#include <sys/types.h>

#define ESC_KEY 27
typedef int socklen_t;

typedef struct JsonData JsonData;

// For OpenSSL checking at the start of the program
#ifdef _WIN32
	#include <io.h>
	#define F_OK 0
	#define access _access
#else
	#include <unistd.h>
#endif

#ifdef _WIN32
#define close_socket(s) closesocket(s)
#else
#define close_socket(s) close(s)
#endif

const char* known_paths[] = {
	#ifdef _WIN32
		"C:\\Program Files\\OpenSSL-Win64\\bin\\openssl.exe",
		"C:\\Program Files (x86)\\OpenSSL-Win32\\bin\\openssl.exe",
		"C:\\OpenSSL-Win64\\bin\\openssl.exe",
		"C:\\Program Files\\Git\\usr\\bin\\openssl.exe" // Often bundled with Git
	#else
		"/usr/bin/openssl",
		"/usr/local/bin/openssl",
		"/opt/homebrew/bin/openssl",
		"/usr/local/ssl/bin/openssl"
	#endif
};

static const char* localhost = "127.0.0.1";

void setup_openssl()
{
	int found = 0;

	// Quick search with system search
	#ifdef _WIN32
		if (system("where openssl >nul 2> nul") == 0) found = 1;
	#else
		if (system("command -v openssl >/dev/null 2>&1") == 0) found = 1;
	#endif

	// If not in path, check common installation folders
	if (!found) {
		int num_paths = sizeof(known_paths) / sizeof(known_paths[0]);
		for (int i = 0; i < num_paths; i++) {
			if (access(known_paths[i], F_OK) == 0) {
				printf("found OpenSSL at: %s\n", known_paths[i]);
				found = 1;
				break;
			}
		}
	}

	if (!found) {
		printf("OpenSSL not found. Running installer...\n");
		#ifdef _WIN32
			system("winget install openssl");
		#else
			system("sudo apt update && sudo apt install -y openssl");
		#endif
	}
	else {
		printf("OpenSSL found.\n\n");
	}
}

void* get(Array* a, int index)
{
	return (char*)a->data + index * a->elem_size;
}

void push_back(Array* a, void* element)
{
	if (a->size == a->capacity) {
		a->capacity *= 2;
		a->data = realloc(a->data, a->capacity * a->elem_size);
	}

	void* target = (char*)a->data + a->size * a->elem_size;
	memcpy(target, element, a->elem_size);

	a->size++;
}

void init_array(Array* a, size_t elem_size)
{
	a->size = 0;
	a->capacity = 2;
	a->elem_size = elem_size;
	a->data = malloc(a->capacity * elem_size);
}

Object* getObject(Array* a)
{
	return object;
}

JsonData* checkValidJSON(JsonData* jsondata) {
	if (!jsondata) {
		printf("Invalid JsonData object");
		return NULL; // basically invalid
	}

	if (!jsondata->device_id) {
		printf("JSON_ERROR: pointer to char 'device_id' is null");
		return NULL;
	}
	else if (!jsondata->last_seen) {
		perror("JSON_ERROR: pointer to char 'last_seen' is null");
		return NULL;
	}
	else if (!jsondata->status) {
		perror("JSON_ERROR: pointer to char 'status' is null");
	}

	// don't need to check for uptime as it is allowed to be zero

	return jsondata;
}

void create_object(Object* object, JsonData* jsondata)
{
	if (checkValidJSON(jsondata)) {
		object->data.device_id = jsondata->device_id;
		object->data.last_seen = jsondata->last_seen;
		object->data.status = jsondata->status;
		object->data.uptime = jsondata->uptime;
	}
}

bool printJSON(Object* object)
{
	if (object) {
		printf("device_id: \%s\n", object->data.device_id);
		printf("status: \%s\n", object->data.status);
		printf("uptime: %d\n", object->data.uptime);
		printf("%s\n", object->data.last_seen);
		return true;
	}

	return false;
}

int messageArrived(void* context, char* topicName, int topicLen, MQTTClient_message* message)
{
	printf("Message arrived %s \n", (char*)message->payload);
	return 1;
}

void init_mqtt(Object* object, MQTTInitOptions* init_options, MQTTClient_connectOptions* connect_options, MQTTClient_SSLOptions* ssl_options)
{
	int rc = 1; // return code
	int qos = 1;
	int retained = 0;
	if (!init_options)
	{
		printf("MQTT initialization options returned null");
		exit(EXIT_FAILURE);
	}
	if (!connect_options)
	{
		printf("MQTT connect options pointer returned null");
		exit(EXIT_FAILURE);
	}
	
	init_options->serverURI = "ssl://quakemonitor-ed51e1cb.a03.euc1.aws.hivemq.cloud:8883";
	init_options->clientID = "cladire_001";

	MQTTClient_create(&object->MClient, init_options->serverURI, init_options->clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	int i = MQTTClient_setCallbacks(object->MClient, NULL, NULL, messageArrived, NULL);
	printf("\ncallback: %d \n", i); // 0 signals a successful connection

	ssl_options->enableServerCertAuth = 0;

	// necessary for TLS
	ssl_options->verify = 1;
	ssl_options->CApath = NULL;
	ssl_options->keyStore = NULL;
	ssl_options->trustStore = NULL;
	ssl_options->privateKey = NULL;
	ssl_options->privateKeyPassword = NULL;
	ssl_options->enabledCipherSuites = NULL;

	connect_options->ssl = ssl_options;
	connect_options->keepAliveInterval = 10;
	connect_options->cleansession = 1;
	connect_options->username = "cladire_001";
	connect_options->password = "Admin1122";
	connect_options->MQTTVersion = MQTTVERSION_3_1_1;

	int j = MQTTClient_connect(object->MClient, connect_options);
	printf("is connected %d \n", rc);

	//if (rc = MQTTClient_connect(object->MQTTClient, &connect_options) != MQTTCLIENT_SUCCESS) {
	//	/*printf("\nMQTT connection failed.\n");
	//	printf("Error code: %d\n", rc);
	//	exit(EXIT_FAILURE);*/
	//}
}

void bind_socket(SOCKET* server_socket)
{
	struct sockaddr_in server_addr;
	memset(&server_addr, 0, sizeof(server_addr));

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(8000); // Bind to port 8000 which is Alt HTTP port
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;

	bind(*server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
}

int init_winsock()
{
	WSADATA wsa;
	int res = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (res != 0) {
		printf("Winsock initialization failed. Error code: %d", WSAGetLastError());
		return 0;
	}
	return 1;
}

size_t write_data(void* ptr, size_t size, size_t nmemb, void* stream)
{
	fwrite(ptr, size, nmemb, stdout);
	return size * nmemb;
}

void init_curl(CURL* curl)
{
	curl_easy_setopt(curl, CURLOPT_URL, "https://127.0.0.1");
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_perform(curl);
}

void cleanup_curl(CURL* curl)
{
	curl_easy_cleanup(curl);
}

// make sure curl is a valid handle
CURLcode HTTP_post_request(CURL* curl, const char* url, const char* body)
{
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_POST, 1L);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

	return curl_easy_perform(curl);
}

// make sure curl is a valid handle
CURLcode HTTP_put_request(CURL* curl, const char* url, const char* body)
{
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);

	return curl_easy_perform(curl);
}

int quakemonitor_run()
{
	// First we just want to make sure we have OpenSSL installed as the app will not work without it
	setup_openssl();

	object = malloc(sizeof(struct Object));
	init_object(object, sizeof(Object));

	json = 0;

	char* json_file_content = load_file("src/json.json");

	if (json_file_content != NULL) {
		parse_json(json_file_content, object, json);

		// we allocated memory in the load_file so let's free it
		free(json_file_content);
	}
	else {
		printf("Json file content is null, returning\n");
		return 1;
	}

	if (printJSON(object) == false) printf("Something went wrong when printing JSON\n");

	MQTTInitOptions init_options;
	MQTTClient_connectOptions connect_options = MQTTClient_connectOptions_initializer;
	MQTTClient_SSLOptions ssl_options = MQTTClient_SSLOptions_initializer;
	//int rc;
	init_mqtt(object, &init_options, &connect_options, &ssl_options);
	if (!init_winsock()) {
		return 1;
	}

	curl = curl_easy_init();
	init_curl(curl);

	return 0;

	// TODO: Probably create a Python interface for quick setup. DASHBOARD WEB (harta + status cladiri)
	// Add Database 
	// Add Node.js API?
	// Monitorizare (rules engine) [Nu stiu ce inseamna asta dar vad]

	// total 800 cladiri: 
	// hardware: 240 000 - 320 000 lei
	// 48K-64K euro
}

int quakemonitor_update()
{
	return 0;
}

void quakemonitor_cleanup()
{
	if (curl) {
		curl_easy_cleanup(curl);
	}

	if (object) {
		free(object);
	}

	if (json) {
		cJSON_Delete(json);
	}
}

// make sure curl is a valid handle
CURLcode HTTP_get_request(CURL* curl, const char* url)
{
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
	
	return curl_easy_perform(curl);
}

int main(int argc, char* argv[])
{
	return quakemonitor_run();
}