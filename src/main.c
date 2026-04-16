#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "../res/cJSON.h"

#include "MQTTClient.h"

//#include 

// For OpenSSL checking at the start of the program
#ifdef _WIN32
	#include <io.h>
	#define F_OK 0
	#define access _access
#else
	#include <unistd.h>
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
	if (checkValidJSON(jsondata)) object->data = *jsondata;

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

	MQTTClient_create(&object->MQTTClient, init_options->serverURI, init_options->clientID, MQTTCLIENT_PERSISTENCE_NONE, NULL);
	int i = MQTTClient_setCallbacks(object->MQTTClient, NULL, NULL, messageArrived, NULL);
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

	connect_options->ssl = &ssl_options;
	connect_options->keepAliveInterval = 10;
	connect_options->cleansession = 1;
	connect_options->username = "cladire_001";
	connect_options->password = "Admin1122";
	connect_options->MQTTVersion = MQTTVERSION_3_1_1;

	int j = MQTTClient_connect(object->MQTTClient, &connect_options);
	printf("is connected %d \n", rc);

	//if (rc = MQTTClient_connect(object->MQTTClient, &connect_options) != MQTTCLIENT_SUCCESS) {
	//	/*printf("\nMQTT connection failed.\n");
	//	printf("Error code: %d\n", rc);
	//	exit(EXIT_FAILURE);*/
	//}
}

int main(int argc, char* argv[])
{
	// First we just want to make sure we have OpenSSL installed as the app will not work without it
	setup_openssl();

	Object* object = malloc(sizeof(Object));
	cJSON* json = 0;

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
	
	// Free the memory on the heap
	free(object);
	cJSON_Delete(json);

	return 0;
}