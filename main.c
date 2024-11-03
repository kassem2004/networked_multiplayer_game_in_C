#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include "include/game_logic.h"

#define MAX_PLAYERS 2
#define BUF_SIZE 1024

void server(){
    int fd;
    int p1_grid[10][10];
    int p2_grid[10][10];
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { // create socket
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }
    
    struct sockaddr_in addr; //bind socket to any open port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;

    socklen_t addr_len = sizeof(addr);

    if(bind(fd, (struct sockaddr*) &addr, addr_len)){
        perror("bind error:");
        pclose(fd);
        return;
    }

    getsockname(fd, (struct sockaddr*) &addr, &addr_len);
    printf("server is on port %d\n", (int) ntohs(addr.sin_port)); //print server port for client to recieve

    if(listen(fd, MAX_PLAYERS)){ //listen to clients
        perror("Listen failed");
        pclose(fd);
        exit(EXIT_FAILURE);
    }
    printf("Waiting for player 1 to connect...\n");

    struct sockaddr_storage p1addr; //client addr
    socklen_t p1addr_len = sizeof(p1addr);
    int p1fd = accept(fd, (struct sockaddr*) &p1addr, &p1addr_len); //accept client packet
    if (p1fd < 0) { 
        printf("server accept failed..."); 
        pclose(fd);
        exit(EXIT_FAILURE); 
    } else {
        printf("Player 1 connected.\n");
    }

    printf("Waiting for player 2 to connect...\n");
    struct sockaddr_storage p2addr;
    socklen_t p2addr_len = sizeof(p2addr);
    int p2fd = accept(fd, (struct sockaddr*) &p2addr, &p2addr_len);
    if (p2fd < 0) { 
        printf("server accept failed..."); 
        pclose(fd);
        exit(EXIT_FAILURE); 
    } else {
        printf("Player 2 connected.\n");
    }

    char buffer[BUF_SIZE];
    strcpy(buffer, "Game Session Began! Each player please set up your boards based on instructions provided.\n");
    send(p1fd, buffer, strlen(buffer), 0);
    send(p2fd, buffer, strlen(buffer), 0);

    recv(p1fd, buffer, BUF_SIZE, 0);
    printf("Received setup confirmation from Player 1.\n");
    recv(p2fd, buffer, BUF_SIZE, 0);
    printf("Received setup confirmation from Player 2.\n");


    close(fd);
    close(p1fd);
    close(p2fd);
}

void client(int port){
    int fd;
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { //create client socket
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if(connect(fd, (struct sockaddr *) &addr, sizeof(addr))){
        perror("Connection failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    char buffer[BUF_SIZE];
    if (recv(fd, buffer, BUF_SIZE, 0) > 0) { //recieve initiation of game
            printf("%s\n", buffer);
    }
    memset(buffer, 0, sizeof(buffer));

    board_setup();

    int setup_count = 0;
    while (setup_count < 5) {
        char player_input[20];
        printf("Enter ship placement: ");
        scanf("%s", player_input);
        setup_count++;
    }

    strcpy(buffer, "Done\n");
    send(fd, buffer, strlen(buffer), 0);
    close(fd);
}


int main(int argc, char *argv[]){
    if (argc > 1 && !strcmp(argv[1], "client")) {
        if (argc != 3) {
            fprintf(stderr, "not enough args!");
            return -1;
        }
    }

    if (strcmp(argv[1], "server") == 0) {
        server();
    } else if (strcmp(argv[1], "client") == 0) {
        int port;
        sscanf(argv[2], "%d", &port);
        client(port);
    } else {
        fprintf(stderr, "Invalid argument. Use 'server' or 'client'.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}