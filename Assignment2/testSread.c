#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define __NR_hello 436
#define __NR_swrite 437
#define __NR_sread 438

#define BUFSIZE 100

void to_binary(char* buf) {
	unsigned char p;

	for(int j = 0; j < strlen(buf); ++j) {
		p = 0x80;
		for(int i = 0; i < 8; ++i) {
			(buf[j] & p) ? printf("1") : printf("0");
			p = p >> 1;
		}
		printf(" ");
	}
	printf("\n");
}

int main(void) {
	int n = 0;
	
	char* temp1,* temp2; int fd;

	temp1 = (char*)calloc(100, sizeof(char));
	temp2 = (char*)calloc(100, sizeof(char));

	if( 0 < ( fd = open("./test.txt", O_RDONLY, 0644))) {
		printf("call read\n");
		read(fd, temp1, BUFSIZE);
		close(fd);
	}
	else {
		printf("Open Error\n");
		exit(-1);
	}

	if( 0 < ( fd = open("./test.txt", O_RDONLY, 0644))) {
		printf("call sread\n");
		syscall(__NR_sread, fd, temp2, BUFSIZE);
		close(fd);
	}
	else {
		printf("Open Error\n");
		exit(-1);
	}

	printf("sys_read() data : %s\n", temp1);
	printf("sys_read() binary data : "); to_binary(temp1);

	printf("sys_sread() data : %s\n", temp2);
	printf("sys_sread() binary data : "); to_binary(temp2);
	
	return 0;
}
