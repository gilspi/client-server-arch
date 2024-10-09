#ifndef DATA_PACKET_H
#define DATA_PACKET_H

#include <stdint.h>

struct DataPacket {
    struct Header {
        uint32_t id;
        double version;
        char description[100];
    } header;

    struct Data {
        int numbers[50];
        char strings[5][50];
    } data;

    uint8_t reserved[64];
};

void generateRandomData(struct DataPacket* packet);

#endif // DATA_PACKET_H

