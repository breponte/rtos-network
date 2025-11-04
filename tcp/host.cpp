#include <bits/stdc++.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <arpa/inet.h> 
#include <netinet/in.h> 

using namespace std;

#define PORT    8080 
#define MAXLINE 1024 

class Host {
    private:
        int sockfd;
        char buffer[MAXLINE]; 
        const char *hello = "Hello from server"; 
        struct sockaddr_in hostaddr, cliaddr; 

    public:
        
        Host (const char * host, const char * client) {
            // create socket file descriptor for I/O access
            if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
                perror("socket creation failed");
                exit(EXIT_FAILURE);
            }

            // clears garbage values from addresses
            memset(&hostaddr, 0, sizeof(hostaddr));
            memset(&cliaddr, 0, sizeof(cliaddr));

            // fill server information
            hostaddr.sin_family = AF_INET;          // IPv4
            hostaddr.sin_addr.s_addr = inet_addr(host); 
            hostaddr.sin_port = htons(PORT);        // htons ensures network \
                                                        byte order

            // fill client information
            cliaddr.sin_family = AF_INET;          // IPv4
            cliaddr.sin_addr.s_addr = inet_addr(client); 
            cliaddr.sin_port = htons(PORT);        // htons ensures network \
                                                        byte order
            
            // binds socket to an actual IPv4 address
            if ( bind(sockfd, (const struct sockaddr *)&hostaddr, 
                    sizeof(hostaddr)) < 0 ) {
                perror("bind failed");
                exit(EXIT_FAILURE);
            }
        }
        ~Host () {
            close(sockfd);
        }

        // NOTE: there exists a message buffer on the network stack, \
            so if a message was received but wasn't processes, it can be later

        int receiveMessage() {
            socklen_t len = sizeof(cliaddr);
            int n;

            n = recvfrom(
                sockfd,
                (char *)buffer,
                MAXLINE,
                MSG_WAITALL,
                (struct sockaddr *) &cliaddr,
                &len
            );
            buffer[n] = '\0';

            cout << "Client : " << buffer << endl;
            return n;
        }

        int sendMessage() {
            socklen_t len = sizeof(cliaddr);

            sendto(
                sockfd,
                (const char *)hello,
                strlen(hello),  
                MSG_CONFIRM,
                (const struct sockaddr *) &cliaddr, 
                len
            );

            cout << "Sending : " << hello << endl;
            return 0;
        }
};