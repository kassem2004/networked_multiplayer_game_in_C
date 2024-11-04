// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include "include/game_logic.h"
#include "include/server.h"

#define MAX_PLAYERS 2
#define BUF_SIZE 1024

void server() {
    int fd, p1fd, p2fd;
    int p1_grid[10][10] = {0};
    int p2_grid[10][10] = {0};
    char buffer[BUF_SIZE];

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { //create socket
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr; //bind socket to any open port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t addr_len = sizeof(addr);

    if (bind(fd, (struct sockaddr*)&addr, addr_len) < 0) { //bind socket to port
        perror("Bind error:");
        close(fd);
        return;
    }

    getsockname(fd, (struct sockaddr*)&addr, &addr_len);
    printf("Server is on port %d\n", ntohs(addr.sin_port));

    if (listen(fd, MAX_PLAYERS) < 0) {
        perror("Listen failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    printf("Waiting for player 1 to connect...\n");
    p1fd = accept(fd, NULL, NULL); //accept p1
    if (p1fd < 0) {
        perror("Server accept failed...");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Player 1 Connected.\n");
    send(p1fd, "One", strlen("One"), 0);

    printf("Waiting for player 2 to connect...\n");
    p2fd = accept(fd, NULL, NULL); //accept p2
    if (p2fd < 0) {
        perror("Server accept failed...");
        close(fd);
        exit(EXIT_FAILURE);
    }
    printf("Player 2 Connected.\n");
    send(p2fd, "Two", strlen("Two"), 0);

    //game begin
    strcpy(buffer, "Game Session Began! Please set up your boards.");
    send(p1fd, buffer, strlen(buffer), 0);
    send(p2fd, buffer, strlen(buffer), 0);
    memset(buffer, 0, BUF_SIZE);

    for (int i = 0; i < 5; i++) { //recieve board placements from player 1
        recv(p1fd, buffer, BUF_SIZE, 0);
        add_to_board(p1_grid, buffer);
        memset(buffer, 0, BUF_SIZE);
    }

    for (int i = 0; i < 5; i++) { //recieve board placements from player 2
        recv(p2fd, buffer, BUF_SIZE, 0);
        add_to_board(p2_grid, buffer);
        memset(buffer, 0, BUF_SIZE);
    }

    print_board(p1_grid);
    print_board(p2_grid);

    close(fd);
    close(p1fd);
    close(p2fd);
}
