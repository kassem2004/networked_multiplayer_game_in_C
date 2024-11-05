#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "include/game_logic.h"
#include "include/client.h"

#define BUF_SIZE 1024

void client(int port) {
    int fd;
    char buffer[BUF_SIZE];
    int board[10][10] = {0};

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { //create socket
        perror("Socket failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((short) port);

    if (inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        close(fd);
        exit(EXIT_FAILURE);
    }

    if (connect(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("Connection failed");
        close(fd);
        exit(EXIT_FAILURE);
    }

    recv(fd, buffer, BUF_SIZE, 0); //find out if you are 1 or 2
    int player = (strcmp(buffer, "One") == 0) ? 1 : 2;
    memset(buffer, 0, BUF_SIZE);

    recv(fd, buffer, BUF_SIZE, 0); //game session began
    printf("%s\n", buffer);
    memset(buffer, 0, BUF_SIZE);
    board_setup();

    for (int i = 0; i < 5; i++) { //send board game setup
        char player_input[25];
        printf("Enter ship placement: ");
        fgets(player_input, sizeof(player_input), stdin);
        send(fd, player_input, strlen(player_input), 0);
    }

    //game started
    while(1){
        char player_move[4];
        recv(fd, buffer, BUF_SIZE, 0);
        if(!strcmp(buffer, "Play!")){
            memset(buffer, 0 ,BUF_SIZE);
            printf("Enter coordinate of attack: ");
            fgets(player_move, sizeof(player_move), stdin);
            send(fd, player_move, strlen(player_move), 0);
            recv(fd, buffer, BUF_SIZE, 0); 
            if(!strcmp(buffer, "Hit!")){
                printf("You Hit!\n");
            } else if (!strcmp(buffer, "Miss!")){
                printf("You Missed!\n");
            }
        }
    }

    close(fd);
}