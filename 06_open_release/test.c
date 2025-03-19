#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv) {
    int fd;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Open the file in read-only mode
    fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error opening file for reading");
        return EXIT_FAILURE;
    }
    close(fd);

    // Open the file in write-only mode, non-blocking
    fd = open(argv[1], O_WRONLY | O_NONBLOCK);
    if (fd < 0) {
        perror("Error opening file for writing");
        return EXIT_FAILURE;
    }

    close(fd);
    return 0;
}
