#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/server.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || strcmp(argv[1], "server") != 0) {
        fprintf(stderr, "Usage: %s server\n", argv[0]);
        return -1;
    }
    server();
    return 0;
}
