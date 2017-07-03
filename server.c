/*
 * Project: c_stager
 * File: server.c
 * Created by Thomas John Wesolowski on 6/2/2017
 */

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>

typedef unsigned char byte_t;

int main(int argc, char **argv) {
    /*
     * Declare socket file descriptors for socket syscall, and accept syscall.
     * Also declare variable to hold port number information.
     * Also declare variable to hold size of client address, and declare
     * another variable to hold result of syscalls like read and write.
     */
    int sockfd, acceptfd, portn, len_cli, res;


    //declare variable to store shellcode and length of shellcode
    //declare byte buffer to store transferred information
    byte_t buf[256];

    //declare structs to store address of server and client
    struct sockaddr_in serv_addr, cli_addr;

    if(argc<2){
        fprintf(stderr,"Please specify port.");
        exit(1);
    }
    //create socket in the internet domain using tcp protocol, and port number
    //provided by user
    sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0){
        fprintf(stderr,"Could not open socket.");
        exit(2);
    }
    //set all of struct serv_addr to 0
    bzero((char *) &serv_addr, sizeof(serv_addr));

    portn = atoi(argv[1]);
    //TODO fix htons call by adding error checking to verify port number is valid.
    //Add address description information
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portn);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    //Bind socket to address
    listen(sockfd,5);

    len_cli = sizeof(cli_addr);
    acceptfd = accept(sockfd,(struct sockaddr*) &cli_addr,(socklen_t *) &len_cli);

    bzero(buf,256);

    //TODO add code here to send shellcode size of fixed length of 4 bytes
    //TODO add code to transmit shellcode to connected client.


    return 0;
}