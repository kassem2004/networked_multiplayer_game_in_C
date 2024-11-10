#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/client.h"

int main(int argc, char *argv[]) {
    if (argc != 3 || strcmp(argv[1], "client") != 0) {
        fprintf(stderr, "Usage: %s client <port>\n", argv[0]);
        return -1;
    }
    int port;
    sscanf(argv[2], "%d", &port);
    client(port);
    return 0;
}
