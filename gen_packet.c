#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "data_packet.h"

void generateRandomData(struct DataPacket* packet) {
    packet->header.id = rand() % 1000;
    packet->header.version = (double)rand() / RAND_MAX * 10.0;
    strcpy(packet->header.description, "Random packet");

    for (int i = 0; i < 50; i++) {
        packet->data.numbers[i] = rand() % 1000;
    }

    const char* sampleStrings[5] = {
        "Sample 1", "Sample 2", "Sample 3", "Sample 4", "Sample 5"
    };

    for (int i = 0; i < 5; i++) {
        strcpy(packet->data.strings[i], sampleStrings[i]);
    }

    memset(packet->reserved, 0, sizeof(packet->reserved));
}
