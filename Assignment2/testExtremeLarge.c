#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <errno.h>

#define __NR_hello 436
#define __NR_swrite 437
#define __NR_sread 438

#define BUFSIZE 10000

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
	int n = 0; int wrote;
	n = syscall(__NR_hello, 9, 7);
	printf("return value : %d\n", n);
	
	char* temp1, * temp2, * temp3; int fd;

	temp1 = (char*)calloc(BUFSIZE, sizeof(char));
	temp2 = (char*)calloc(BUFSIZE, sizeof(char));
	temp3 = (char*)calloc(BUFSIZE, sizeof(char));	


	if( 0 < ( fd = open("./systable.txt", O_RDONLY, 0644))) {
		printf("read systable\n");
		read(fd, temp1, BUFSIZE);
		close(fd);
	}

	if( 0 < ( fd = open("./systableEncrypted.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644))) {
		printf("call swrite\n");
		wrote = syscall(__NR_swrite, fd, temp1, strlen(temp1));
		wrote > 0 ? printf("wrote : %d\n", wrote) : printf("error : %s\n", strerror(-wrote));
		close(fd);
	}
	else {
		printf("Open Error\n");
		exit(-1);
	}

	if( 0 < ( fd = open("./systableEncrypted.txt", O_RDONLY, 0644))) {
		read(fd, temp2, BUFSIZE);
		close(fd);
	}
	else {
		printf("Open Error\n");
		exit(-1);
	}

	printf("sys_read() data :\n %s\n", temp2);
	printf("sys_read() binary data :\n"); to_binary(temp2);

	if ( 0 < ( fd = open("./systableEncrypted.txt", O_RDONLY, 0644))) {
		wrote = syscall(__NR_sread, fd, temp3, BUFSIZE);
		wrote > 0 ? printf("read : %d\n", wrote) : printf("error : %s\n", strerror(-wrote));
		close(fd);
	}
	else {
		printf("Open Error\n");
		exit(-1);
	}

	printf("sys_sread() data :\n %s\n", temp3);
	printf("sys_sread() binary data :\n"); to_binary(temp3);

	return 0;
}
