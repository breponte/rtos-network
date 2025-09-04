#include "tcp_embed.hpp"
#include "sensors.hpp"

HandshakeState handleTCPState(HandshakeState state, EventTCP event) {
    switch (state)
    {
    case CLOSED:
        if (event == Send) {
            // TODO: Send SYN
            return SYN_SENT;
        } else {
            return LISTEN;
        }
    case LISTEN:
        if (event == SYN) {
            // TODO: Send SYN_ACK
            return SYN_RCVD;
        } else if (event == Send) {
            // TODO: Send SYN
            return SYN_SENT;
        } else {
            return LISTEN;
        }
    case SYN_SENT:
        if (event == Close) {
            return CLOSED;
        } else if (event == SYN) {
            // TODO: Send SYN_ACK
            return SYN_RCVD;
        } else if (event == SYN_ACK) {
            // TODO: Send ACK
            return ESTABLISHED;
        } else {
            return SYN_SENT;
        }
    case SYN_RCVD:
        if (event == ACK) {
            return ESTABLISHED;
        } else if (event == Close) {
            // TODO: Send FIN
            return FIN_WAIT_1;
        } else {
            return SYN_RCVD;
        }
    case ESTABLISHED:
        if (event == Close) {
            // TODO: Send FIN
            return FIN_WAIT_1;
        } else if (event == FIN) {
            // TODO: Send ACK
            return CLOSE_WAIT;
        } else {
        // TODO: Send message
            return ESTABLISHED;
        }
    case FIN_WAIT_1:
        if (event == ACK) {
            return FIN_WAIT_2;
        } else if (event == FIN_ACK) {
            // TODO: Send ACK
            return TIME_WAIT;
        } else if (event == FIN) {
            // TODO: Send ACK
            return CLOSING;
        } else {
            return FIN_WAIT_1;
        }
    case FIN_WAIT_2:
        if (event == FIN) {
            // TODO: Send ACK
            return TIME_WAIT;
        } else {
            return FIN_WAIT_2;
        }
    case CLOSING:
        if (event == ACK) {
            return TIME_WAIT;
        } else {
            return CLOSING;
        }
    case TIME_WAIT:
        // TODO: Timeout
        return TIME_WAIT;
    case CLOSE_WAIT:
        if (event == Close) {
            // TODO: Send FIN
            return LAST_ACK;
        } else {
            return CLOSE_WAIT;
        }
    case LAST_ACK:
        if (event == ACK) {
            return CLOSED;
        } else {
            return LAST_ACK;
        }

    default:
        // unrecognized state
        return state;
    }
}

void tcpSendMessage(uint16_int flags) {
    int n;
    socklen_t len;

    if (xSemaphoreTake(mutexMotor, 0) == pdFALSE) return;
    if (xSemaphoreTake(mutexInfrared, 0) == pdFALSE) return;
    if (xSemaphoreTake(mutexPhotoresistor, 0) == pdFALSE) return;
    
    PacketTCP packet;
    packet.flags = flags;
    // TODO: Record and input sequenceNum and ackNum
    // TODO: Record and input window size
    // TODO: Urgent pointer???
    // TODO: CRC checksum

    // WARNING: Data misalignment, manage directly
    memcpy(&packet.data, &sensors, sizeof(SensorStates));

    // little endian
    sendto(sockfd, (const char *)packet, sizeof(PacketTCP), 
        0, (const struct sockaddr *) &servaddr,  
            sizeof(servaddr));
    
    xSemaphoreGive(mutexPhotoresistor);
    xSemaphoreGive(mutexInfrared);
    xSemaphoreGive(mutexMotor);
            
    n = recvfrom(sockfd, (char *)inBuffer, MAXLINE,  
                MSG_WAITALL, (struct sockaddr *) &servaddr, 
                &len); 
    inBuffer[n] = '\0'; 
    Serial.println(inBuffer);
}

void tcpTask(void *pvParameters) {
    while (true) {
        tcpSendMessage();
        vTaskDelay(pdMS_TO_TICKS(100));
    }  
}