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

    int p1_count = 0;
    char *p1_placed_pieces[5] = {NULL};
    /*for(int i = 0; i < 5; i++){
        p1_placed_pieces[i] = malloc(15 * sizeof(char));
    }
    for (int i = 0; i < 5; i++) {
        memset(p1_placed_pieces[i], 0, 15);
    }*/
    while(p1_count < 5) { //recieve board placements from player 1
        recv(p1fd, buffer, BUF_SIZE, 0);
        if(add_to_board(p1_grid, buffer, p1_placed_pieces, &p1_count) == 1){
            memset(buffer, 0, BUF_SIZE);
            strcpy(buffer, "Valid");
            send(p1fd, buffer, strlen(buffer), 0);
            p1_count++;
        } else {
            memset(buffer, 0, BUF_SIZE);
            strcpy(buffer, "Invalid");
            send(p1fd, buffer, strlen(buffer), 0);
        }
        memset(buffer, 0, BUF_SIZE);
    }
    /*
    for(int i = 0; i < 5; i++){
        free(p1_placed_pieces[i]);
    }*/

    int p2_count = 0;
    char *p2_placed_pieces[5] = {NULL};
    while(p2_count < 5) { //recieve board placements from player 2
        recv(p2fd, buffer, BUF_SIZE, 0);
        if(add_to_board(p2_grid, buffer, p2_placed_pieces, &p2_count) == 1){
            memset(buffer, 0, BUF_SIZE);
            strcpy(buffer, "Valid");
            send(p2fd, buffer, strlen(buffer), 0);
            p2_count++;
        } else {
            memset(buffer, 0, BUF_SIZE);
            strcpy(buffer, "Invalid");
            send(p2fd, buffer, strlen(buffer), 0);
        }
        memset(buffer, 0, BUF_SIZE);
    }

    int outcome = -1;

    //game started
    while(1){
        strcpy(buffer, "Play!");
        send(p1fd, buffer, strlen(buffer), 0); //signal to play move
        memset(buffer, 0, BUF_SIZE);
        recv(p1fd, buffer, BUF_SIZE, 0); //move from p1
        outcome = play_move(p2_grid, buffer);
        if(outcome){
            strcpy(buffer, "Hit!");
            send(p1fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUF_SIZE);
        } else {
            strcpy(buffer, "Miss!");
            send(p1fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUF_SIZE);
        }

        strcpy(buffer, "Play!");
        send(p2fd, buffer, strlen(buffer), 0); //signal to start playing
        memset(buffer, 0, BUF_SIZE);
        recv(p2fd, buffer, BUF_SIZE, 0); //move from p2
        outcome = play_move(p1_grid, buffer);
        if(outcome){
            strcpy(buffer, "Hit!");
            send(p2fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUF_SIZE);
        } else {
            strcpy(buffer, "Miss!");
            send(p2fd, buffer, strlen(buffer), 0);
            memset(buffer, 0, BUF_SIZE);
        }
    }

    close(fd);
    close(p1fd);
    close(p2fd);
}
