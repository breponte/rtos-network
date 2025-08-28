#include "udp_embed.hpp"
#include "sensors.hpp"

int sockfd = -1;
char outBuffer[sizeof(SensorStates)] = ""; 
char inBuffer[MAXLINE] = "";
struct sockaddr_in servaddr = {};

void udpSetup() {
    // Creating socket file descriptor 
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) { 
        perror("socket creation failed"); 
        exit(EXIT_FAILURE); 
    } 

    memset(&servaddr, 0, sizeof(servaddr)); 

    // Filling server information 
    servaddr.sin_family = AF_INET; 
    servaddr.sin_port = htons(PORT); 
    servaddr.sin_addr.s_addr = inet_addr("192.168.1.116"); 
}

void udpTask(void *pvParameters) {
    while (true) {
        int n;
        socklen_t len;

        if (xSemaphoreTake(mutexMotor, 0) == pdFALSE) continue;
        if (xSemaphoreTake(mutexInfrared, 0) == pdFALSE) continue;
        if (xSemaphoreTake(mutexPhotoresistor, 0) == pdFALSE) continue;
        
        memcpy(outBuffer, &sensors, sizeof(SensorStates));

        // little endian
        sendto(sockfd, (const char *)outBuffer, sizeof(SensorStates), 
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

        vTaskDelay(pdMS_TO_TICKS(100));
    }  
}