#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define _WIN32_WINNT 0x0601

#include <WinSock2.h>
#include <WS2tcpip.h>

//Сборка на Windows с указанием библиотек -lws2_32 -lwsock32

int main()
{

    int server, client;
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    int portNumber = 8000;

    // Если произошла ошибка подгрузки библиотеки
    if (result != 0) {
        printf("WSAStartup failed\n");
        return result;
    }

    struct sockaddr_in server_addr;
    socklen_t size;

    // Создание сокета
    server = socket(AF_INET, SOCK_STREAM, 0);

    if (server == INVALID_SOCKET) {
        printf("Error at socket\n");
        WSACleanup();
        return 1;
    }

    printf("=> Socket server has been created...\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);
    server_addr.sin_port = htons(portNumber);

    result = bind(server, (struct sockaddr*)&server_addr,sizeof(server_addr));

    if ( result == SOCKET_ERROR) {
        printf("bind failed with error\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }
    size = sizeof(server_addr);
    printf("=> Looking for clients...\n");

    // Инициализируем слушающий сокет
    if (listen(server, 1) == SOCKET_ERROR) {
        printf("listen failed with error\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    // Принимаем входящие соединения
    client = accept(server, (struct sockaddr *)&server_addr,&size);
    if (client == INVALID_SOCKET) {
        printf("accept failed\n");
        closesocket(server);
        WSACleanup();
        return 1;
    }

    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];

   if(client > 0) 
    {
        printf("=> Connected with the client , you are good to go...\n");
        printf("\n=> Enter # to end the connection\n");     
        strcpy(buffer, "=> Server connected...\n");
        send(client, buffer, bufsize, 0);

        do {
			printf("Client: ");
            do {
                recv(client, buffer, bufsize, 0);
                printf("%s ",buffer);
                if (buffer[0] == '#') 
                {
                    isExit = true;
                }
            } while (buffer[0] != '.' &&  buffer[0] != '#');                        
                        
            printf("\nServer: ");
            char c;
            int cntr = 0;
            while ((c = getchar()) != '\n')
                buffer[cntr++] = c;
            buffer[cntr++] = 0;
            if (buffer[0] == '#') 
                isExit = true;
            send(client, buffer, bufsize, 0);
            send(client, ".", 2, 0);            

        } while (!isExit);

        printf("\n\n=> Connection terminated with IP %s\n",inet_ntoa(server_addr.sin_addr));    
        printf("\nGoodbye...");
    }
    
    closesocket(server);
    closesocket(client);
    WSACleanup();
    return 0;
}