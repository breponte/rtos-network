#include <cstdint>

typedef struct {
    uint16_t srcPort; uint16_t destPort;
    uint32_t seqNum;
    uint32_t ackNum;
    uint16_t flags; uint16_t windowSz;
    uint16_t checkSum; uint16_t urgentPtr;
    // skip options
    uint64_t data;
} Packet;