#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include "data_packet.h"
#define PORT 8080


void func(int client) {
    struct DataPacket packet;

    ssize_t bytes_read = read(client, &packet, sizeof(packet)); // читаем данные от клиента
    if (bytes_read == -1) {
        perror("Ошибка при чтении данных от клиента");
        return;
    }

    printf("===== Received data from client =====\n");
    printf("Header ID: %u\n", packet.header.id);
    printf("Header Version: %.2f\n", packet.header.version);
    printf("Header Description: %s\n", packet.header.description);

    printf("Numbers in Data:\n");
    for (int i = 0; i < 50; i++) {  // предположительно 50 чисел
        printf("%d ", packet.data.numbers[i]);
    }
    printf("\n");

    printf("Strings in Data:\n");
    for (int i = 0; i < 5; i++) {  // предположительно 5 строк
        printf("String %d: %s\n", i + 1, packet.data.strings[i]);
    }

    printf("Reserved data: ");
    for (int i = 0; i < (int)sizeof(packet.reserved); i++) {  // Преобразуем размер в int
        printf("%02X ", packet.reserved[i]);
    }
    printf("\n========================================\n");

    // Отправляем 100 раз данные обратно клиенту
    for (int i = 0; i < 100; ++i) {
        generateRandomData(&packet);
        ssize_t bytes_written = write(client, &packet, sizeof(packet));
        if (bytes_written == -1) {
            perror("Ошибка при отправке данных клиенту");
            return;
        }
    }
}


int main() {
    int server, client;
    struct sockaddr_in servaddr, cli;
    socklen_t len;

    server = socket(AF_INET, SOCK_STREAM, 0);
    if (server == -1){
        printf("server creation failed....\n");
        exit(EXIT_FAILURE);
    } else {
        printf("server successfully created!\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&servaddr, sizeof(servaddr)) == -1){
        printf("bind creation failed....\n");
        exit(EXIT_FAILURE);
    } else {
        printf("bind successfully created!\n");
    }

    if ((listen(server, 5)) == -1) {
        printf("listen creation failed....\n");
        exit(EXIT_FAILURE);
    } else {
        printf("listen successfully created!\n");
    }

    len = sizeof(cli);
    client = accept(server, (struct sockaddr *)&cli, &len);
    if (client == -1) {
        printf("accept creation failed....\n");
        exit(EXIT_FAILURE);
    } else {
        printf("accept successfully created!\n");
    }

    func(client);

    close(client);
    close(server);
}