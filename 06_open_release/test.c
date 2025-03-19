#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char **argv){
	int fd;

	if (argc < 2){
		printf("File requires to open as an argument\n");
		return 0;
	}

	fd = open(argv[1], O_RDONLY);

	if(fd < 0){
		perror("open");
		return fd;
	}

	close (fd);

	fd = open(argv[1], O_WRONLY | O_NONBLOCK);

	if(fd < 0){
		perror("open");
		return fd;
	}

	close(fd);
	return 0;
}
