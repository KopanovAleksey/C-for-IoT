#include "stdio.h"
#include "stdint.h"
#include "time.h"
#include "stdlib.h"
#include "string.h"

#include "MQTTClient.h"

#define ADDRESS     "192.168.171.128:1883"
#define CLIENTID    "SUB"
#define TOPIC       "GB"
#define PAYLOAD     "32"
#define QOS         0
#define TIMEOUT     10000L
double DELAY = 5;

int main(int argc, char* argv[])
{
	
    FILE *file;
	file = fopen("temperature_receive.csv","w");
    fclose(file);
    
    MQTTClient client;
    MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;
    MQTTClient_createOptions createOpts = MQTTClient_createOptions_initializer;

    int rc;

    rc = MQTTClient_createWithOptions(&client, ADDRESS, CLIENTID, 
										MQTTCLIENT_PERSISTENCE_NONE, 
										NULL, &createOpts);
    conn_opts.keepAliveInterval = 20;
    conn_opts.cleansession = 1;
    conn_opts.username = "IoT";
    conn_opts.password = "KopanovAE";
    

    if ((rc = MQTTClient_connect(client, &conn_opts)) != MQTTCLIENT_SUCCESS)
    {
        printf("Failed to connect, return code %d\n", rc);
        exit(-1);
    }
    printf("Success to connect, return code %d\n", rc);
    rc = MQTTClient_subscribe(client, TOPIC, QOS);
    if (rc != MQTTCLIENT_SUCCESS && rc != QOS){
		fprintf(stderr, "Error %d subscribing to topic %s\n", rc, TOPIC);
		exit(-2);
	}
	printf("Success subscribing to topic %s\n", TOPIC);
	printf("Waiting for msg\n");
    
	while(rc != -1){   
		
		time_t mytime = time(NULL);
		struct tm *now = localtime(&mytime);
        
		char* topicName = TOPIC;
		int topicLen = strlen(PAYLOAD);
		MQTTClient_message* message = NULL;
		
		rc = MQTTClient_receive(client, &topicName, &topicLen, &message, 1000);
		if (rc == MQTTCLIENT_DISCONNECTED){
			printf("!\n");
			rc = MQTTClient_connect(client, &conn_opts);
		}
		else if(message){
			file = fopen("temperature_receive.csv","a+");
			char str[10];
			char result[255];
			sprintf(result,"%d;%d;%d;%d;%d;",
						now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
						now->tm_hour, now->tm_min);
			sprintf(str,"%.*s\n", message->payloadlen, (char*)message->payload);
			strcat(result, str);
			printf("%s",result);
			fwrite(result, sizeof(result[0]), strlen(result), file);
			fclose(file);
			fflush(stdout);
			MQTTClient_freeMessage(&message);
			MQTTClient_free(topicName);
		}
	}
    
    MQTTClient_disconnect(client, 10000);
    MQTTClient_destroy(&client);
    return rc;
}
