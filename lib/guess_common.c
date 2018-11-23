#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "guess_common.h"

int		rand_number(int min, int max)
{
	srand(time(0));
	return rand() % (max - min) + min;
}

void	send_number(int socket, int number)
{
	char		buf[32];
	int			out_msg_len;

	out_msg_len = snprintf(buf, sizeof(buf)/sizeof(buf[0]), "%d\n", number);
	if (write(socket, buf, out_msg_len) < 0)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
}

int	recv_number(int socket)
{
	char		buf[32];
	int			bytes_read;

	bytes_read = read(socket, buf, sizeof(buf) / sizeof(buf[0]) - 1);
	if (bytes_read < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	buf[bytes_read] = '\0';
	return atoi(buf);
}

const char	*status_str(int num)
{
	static const char *msg[] = 
	{
		"error",
		"equal",
		"less",
		"greater"
	};

	return msg[num >= 0 && num < sizeof(msg)/sizeof(msg[0]) ? num : 0];
}
