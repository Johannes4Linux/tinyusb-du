#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

struct cmd {
	char cmd;
	uint8_t val[3];
} __attribute__((packed));

int main(int argc, char **argv)
{
	int fd, i;
	struct cmd report;

	if (argc < 5)
		return 1;

	fd = open(argv[1], O_RDWR);
	if (fd < 0) {
		perror("open");
		return fd;
	}

	report.cmd = 'L';
	for (i = 0; i < 3; i++)
		report.val[i] = atoi(argv[2 + i]);

	write(fd, &report, sizeof(report));

	memset(&report, 0, sizeof(report));
	report.cmd= 'A';
	write(fd, &report, sizeof(report));
	read(fd, &report, sizeof(report));

	printf("%d\n", report.val[0]);




	close(fd);
	return 0;
}
