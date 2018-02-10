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
	ssize_t bytes_read;
	unsigned char buffer[BSIZE];

	int fd = open("file.txt", O_CREAT | O_RDWR);

	if (fd >= 0)
	{
		bytes_read = read(fd, buffer, BSIZE);
		if (bytes_read > 0)
		{
			unsigned char* p = buffer;
			while (p < buffer + bytes_read)
			{
				*(p++) += 1;
			}
			if ((head_pos = lseek(fd, 0, SEEK_SET)) != 0)
			{
				printf("Attempting to seek to beginning returned: %d\n", head_pos);
			}
			else
			{
				if ((bytes_written = write(fd, buffer, bytes_read)) != bytes_read)
				{
					printf("Write returned: %d expected: %d\n", bytes_written, bytes_read);
					perror("Error writing");
				}
			}
		}
		close(fd);
	}
	return 0;
}
