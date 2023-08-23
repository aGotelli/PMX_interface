
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <iostream>

//TCPClient_main hostname portnumber
int main(int argc, char** argv){

    int sockfd, newsockfd, portnum;
    double statsdata[100];
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr; //server and client address

//    printf("Port is:%s\n", argv[1]);

    //port used to communicate with server
    portnum = 5005;

    //create IP domain socket
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
        fprintf(stderr,"Error creating the socket.\n");
    }

    //make sure serv_addr is cleared and then set values for each member
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portnum);
//    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_addr.s_addr = inet_addr("192.168.1.15");

    int binderror = bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (binderror < 0)
        fprintf(stderr, "Error Binding.\n");

    //initiate connect to server address(struct sockaddr *) &serv_addr on socket
    listen(sockfd,1);

    clilen = sizeof(cli_addr);

    int messagelength = 0;
    bool stop = false;

    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

    while(!stop) {

        if ((read(newsockfd, &messagelength, sizeof(messagelength))) < 0) {
            fprintf(stderr,"Error reading data from socket.\n");
            stop = true;
        }

        if ((read(newsockfd, &statsdata, sizeof(statsdata))) < 0)
            fprintf(stderr,"Error reading data from socket.\n");

        std::cout << statsdata << std::endl;

        usleep(100000);
    }


    /*
    while (count < 50){
    //populate statistics data to send to server
    for(int i=0; i<100; i++) {
        statsdata[i] = rand();
    }
    */
    close(newsockfd);
    close(sockfd);

    return 0;
}
