#include <Arduino.h>
#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

#define PORT    8080 
#define MAXLINE 1024 

extern int sockfd; 
extern char buffer[MAXLINE]; 
extern const char *hello; 
extern struct sockaddr_in servaddr;


void udpSetup();

void udpTask(void *pvParameters);