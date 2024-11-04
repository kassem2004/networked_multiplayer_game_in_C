#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/server.h"
#include "include/client.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [server|client <port>]\n", argv[0]);
        return -1;
    }

    if (strcmp(argv[1], "server") == 0) {
        server();
    } else if (strcmp(argv[1], "client") == 0) {
        if (argc != 3) {
            fprintf(stderr, "Client mode requires a port argument.\n");
            return -1;
        }
        int port;
        sscanf(argv[2], "%d", &port);
        client(port);
    } else {
        fprintf(stderr, "Invalid argument. Use 'server' or 'client'.\n");
        return -1;
    }

    return 0;
}
