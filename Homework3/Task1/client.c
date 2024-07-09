#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define _WIN32_WINNT 0x0601

#include <WinSock2.h>
#include <WS2tcpip.h>

//Сборка на Windows с указанием библиотек -lws2_32 -lwsock32

int main()
{
    WSADATA wsaData;
    int client;
    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];
    char* ip = "127.0.0.1";
    int portNumber = 8000;

    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (result != 0) {
        printf("WSAStartup failed\n");
        return result;
    }

    struct sockaddr_in server_addr;
    socklen_t size;

    // Создание сокета
    client = socket(AF_INET, SOCK_STREAM, 0);

    printf("CLIENT\n");
    if (client == INVALID_SOCKET) {
        printf("Error at socket\n");
        WSACleanup();
        return 1;
    }
    printf("=> Socket client has been created...\n");

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNumber);

    inet_pton(AF_INET, ip, &server_addr.sin_addr);

    int connection = connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr));

    if(connection != 0){
        printf("=> Trying connecting to server %s with port number: %d\n",inet_ntoa(server_addr.sin_addr),portNumber);
    }
    while (connection != 0)
    {
        connection = connect(client,(struct sockaddr *)&server_addr, sizeof(server_addr));
    }
     
    if (connection == 0)
        printf("=> Connection to the server %s with port number: %d\n",inet_ntoa(server_addr.sin_addr),portNumber);
    
    printf("=> Awaiting confirmation from the server...\n");
    recv(client, buffer, bufsize, 0);
    printf("=> Connection confirmed, you are good to go...\n");

    printf("\n\n=> Enter # to end the connection\n");
    
    do {
		printf("Client: ");
        char c;
        int cntr = 0;
        while ((c = getchar()) != '\n')
            buffer[cntr++] = c;
        buffer[cntr++] = 0;
        if (buffer[0] == '#') 
            isExit = true;
        send(client, buffer, bufsize, 0);
        send(client, ".", 2, 0);

        printf("Server: ");
        do {
            recv(client, buffer, bufsize, 0);
            printf("%s ",buffer);
            if (buffer[0] == '#') 
            {
                isExit = true;
            }
        } while (buffer[0] != '.' &&  buffer[0] != '#');
        printf("\n");

    } while (!isExit);

    printf("\n=> Connection terminated.\n\nGoodbye...\n");

    closesocket(client);
    WSACleanup();
    return 0;
}