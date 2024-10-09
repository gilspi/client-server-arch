#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <string.h>
#include "data_packet.h"
#define PORT 8080


void func(int client){
    struct DataPacket packet;

    read(client, &packet, sizeof(packet));
    printf("Received from client: ID=%d, Value=%.2f, Message=%s\n", 
           packet.id, packet.value, packet.msg);
    for (int i = 0; i < 100; ++i){
        write(client, &packet, sizeof(packet));
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