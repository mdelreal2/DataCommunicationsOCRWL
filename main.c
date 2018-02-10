#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

inline off_t where_am_i(int fd)
{
	assert(fd >= 0);
	return lseek(fd, 0, SEEK_CUR);
}

int main()
{
	const int BSIZE = 512;
	const int NUM_REWRITES = 4;

	off_t head_pos;
	ssize_t bytes_written;
	unsigned char buffer[BSIZE];

	int fd = open("file.txt", O_CREAT | O_RDWR);

	if (fd >= 0)
	{
		printf("file opened\n");

		int i;
		for (i = 0; i < NUM_REWRITES; i++)
		{
			memset(buffer, i, BSIZE);
			printf("initialized buffer to all %d's\n", i);

			if ((head_pos = lseek(fd, 0, SEEK_SET)) != 0)
			{
				printf("Attempting to seek to beginning returned: %d\n", head_pos);
				break;
			}

			printf("Write head: %d\n", where_am_i(fd));

			if ((bytes_written = write(fd, buffer, BSIZE)) != BSIZE)
			{
				printf("Write returned: %d expected: %d\n", bytes_written, BSIZE);
				perror("Error writing");
				break;
			}

			printf("Wrote block of %d's\n", i);
			printf("Write head: %d\n\n", where_am_i(fd));

		}
		close(fd);
	}
	else
	{
		perror("The file did not open");
	}

	return 0;
}
