#include <stdlib.h>
#include <stdio.h>
#include <sys/io.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include "data_packet.h"

#define PORT 8080


void func(int client) {
    struct DataPacket response;
    struct DataPacket packet;
    // наш пакет данных 
    packet.id = 1;
    packet.value = 1.25;
    strcpy(packet.msg, "Hello, Server!");
    //
    write(client, &packet, sizeof(packet));  // отправили наш пакет на сервер
    /*
    Сервер в ответ должен прислать ответ, который мы прочитаем и выведем его на экран. 
    Ответом будет 100 сообщений от сервера.
    */
    for (int i = 1; i <= 100; ++i){
        bzero(&response, sizeof(response));  // подготовили ячейки памяти к приему данных

        read(client, &response, sizeof(response));
        printf("Received data %d from server: ID=%d\tVALUE=%.3f\tMESSAGE=%s\n", 
                i, response.id, response.value, response.msg);

    }
}



int main(){
    int client;
    struct sockaddr_in servaddr;

    client = socket(AF_INET, SOCK_STREAM, 0);
    if (client == -1){
        printf("server creation failed....\n");
        exit(EXIT_FAILURE);
    } else {
        printf("server successfully created!\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // указываем адрес
    servaddr.sin_port = htons(PORT);  // указываем порт в сетевом порядке байтов
    servaddr.sin_family = AF_INET; // IPv4

    if (connect(client, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1) {
        printf("connect failed...\n");
        exit(EXIT_FAILURE);
    } else {
        printf("connect created!\n");
    }

    func(client);
    close(client);    
}