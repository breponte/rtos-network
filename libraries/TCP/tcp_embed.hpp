#include <Arduino.h>
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 
#include <cstdint>

// denotes state of handshake
enum HandshakeState {
    CLOSED,         // opening communication...
    LISTEN,         // ...
    SYN_SENT,       // ...
    SYN_RCVD,       // ...
    ESTABLISHED,    // established communication...
    FIN_WAIT_1,     // closing communication...
    FIN_WAIT_2,     // ...
    CLOSING,        // ...
    TIME_WAIT,      // ...
    CLOSE_WAIT,     // ...
    LAST_ACK,       // ...
};

// denotes the received flags from remote or action taken by local
enum EventTCP {
    Send,           // local action...
    Close,          // ...
    SYN,            // received flags...
    ACK,            // ...
    SYN_ACK,        // ...
    FIN,            // ...
    FIN_ACK         // ...
};

// helps OR (|) the flags field
enum Flag {
    SYN = 1,
    FIN = 2,
    RESET = 4,
    PUSH = 8,
    URG = 16,
    ACK = 32
};

typedef struct {
    uint32_t  sequenceNum,    // 2^32 possible
    uint32_t  ackNum,         // 2^32 possible
    uint16_t  flags,          // SYN, FIN, RESET, PUSH, URG, and ACK
    uint16_t  windowSize,     // 2^16 max size
    uint16_t  checksum,       // CRC
    uint16_t  urgentPtr,      // ???
    uint64_t  data            // 4 * uint16_t for 3 sensor readings and bools
} PacketTCP;


HandshakeState handleTCPState(HandshakeState state, EventTCP event);
void tcpSendMessage(uint16_int flags);
void tcpTask(void *pvParameters);