
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>

#include "spirit_msg.h"
#include "iostream"

#define MAGIC 0xF239
#define IOCTL_SPIRIT_MSG		_IOWR(MAGIC, 0, SpiritMsg)

int main()
{
	int fd = open("/dev/spiritko", O_RDWR);

	int hep = 737;

	SpiritMsg spmsg;
	spmsg.title = 239;
	spmsg.size = sizeof(hep);
	spmsg.data = &hep;

	if (fd < 0) {
		printf("Drv open failed\n");
		return -1;
	}

	ioctl(fd, IOCTL_SPIRIT_MSG, &spmsg);

	printf("size = %d, chan = %llx\n", spmsg.size, *(long long *)spmsg.data);

	close(fd);

	return 0;
}
