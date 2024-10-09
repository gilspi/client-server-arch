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
    // заполняем наш пакет
    generateRandomData(&packet);
    //
    if (write(client, &packet, sizeof(packet)) == -1){  // отправили наш пакет на сервер
        perror("Ошибка отправки данных на сервер");
        exit(EXIT_FAILURE);
    };
    /*
    Сервер в ответ должен прислать ответ, который мы прочитаем и выведем его на экран. 
    Ответом будет 100 сообщений от сервера.
    */
    for (int i = 1; i <= 100; ++i){
        bzero(&response, sizeof(response));  // подготовили ячейки памяти к приему данных

        ssize_t bytes_read = read(client, &response, sizeof(response));
        if (bytes_read == -1){
            perror("Ошибка при чтении данных с сервера");
            exit(EXIT_FAILURE);
        }

        printf("===== Received data %d from server =====\n", i);
        printf("Header ID: %u\n", response.header.id);
        printf("Header Version: %.2f\n", response.header.version);
        printf("Header Description: %s\n", response.header.description);

        printf("First 5 numbers: ");
        for (int j = 0; j < 5; j++) {
            printf("%d ", response.data.numbers[j]);
        }
        printf("\n");

        printf("Strings in Data:\n");
        for (int j = 0; j < 5; j++) {
            printf("String %d: %s\n", j + 1, response.data.strings[j]);
        }
        printf("\n========================================\n");
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