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

    bool isExit = false;
    int bufsize = 1024;
    char buffer[bufsize];

    while(!isExit)
    {
        client = accept(server,(struct sockaddr *)&server_addr,&size);

        if (client < 0) 
            printf("=> Error on accepting...");

        if(client > 0) 
        {
            printf("=> Connected with the client, you are good to go...\n");


            
            int result = recv(client, buffer, bufsize, 0);
            if (result < 0)
            {
                printf("\n\n=> Connection terminated error %d  with IP %s\n",result,inet_ntoa(server_addr.sin_addr));   
                closesocket(client);
                exit(1);
            }

            buffer[result] = '\0';               
            char response[1024] = "HTTP/1.1 200 OK\r\n"
                "Version: HTTP/1.1\r\n"
                "Content-Type: text/html; charset=utf-8\r\n"
                "\r\n\r\n"
                    "<!DOCTYPE HTML>"
                    "<html>"
                    "  <head>"
                    "    <meta name=\"viewport\" content=\"width=device-width,"
                    "    initial-scale=1\">"
                    "  </head>"
                    "  <h1>Task 3</h1>"
                    "  <p>Button #1"
                    "    <a href=\"on1\">"
                    "      <button>ON</button>"
                    "    </a>&nbsp;"
                    "    <a href=\"off1\">"
                    "      <button>OFF</button>"
                    "    </a>"
                    "  </p>"
                    "  <p>Exit"
                    "    <a href=\"Exit\">"
                    "      <button>Exit</button>"
                    "    </a>"
                    "  </p>";
            strcat(response,"</html>");
            //printf("%s\n",buffer);
            char* get_str_exit = strstr(buffer, "GET /Exit");
            char* get_str_on1 = strstr(buffer, "GET /on1");
            char* get_str_off1 = strstr(buffer, "GET /off1");
            if(get_str_on1){
                printf("Client press on1\n");
            }
            if(get_str_off1){
                printf("Client press off1\n");
            }
            if(get_str_exit){
                printf("Client exit\n");
                isExit = true;
                break;
            }
            send(client, response,strlen(response), 0);
            printf("\n\n=> Connection terminated with IP %s\n",inet_ntoa(server_addr.sin_addr));   
            closesocket(client);
            printf("\n=> Press any key and <Enter>, # to end the connection\n");
            char c;
            while((c=getchar())!='\n')
                if(c=='#')
                {
                    isExit=true;                 
                }
        }
    }
    printf("\nGoodbye...");
    closesocket(server);
    closesocket(client);
    WSACleanup();
    return 0;
}