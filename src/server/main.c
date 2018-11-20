#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

static int create_server_socket(int port, int backlog);
static void accept_clients(int server_socket);

int main(int argc, char *argv[]) 
{
	int		server_socket;

	server_socket = create_server_socket(8000, 10);
	accept_clients(server_socket);
	close(server_socket);

	return 0; 
}

static int create_server_socket(int port, int backlog)
{
	struct sockaddr_in	channel;
	int					server_socket;
	int					opt;

	opt = 1;
	channel.sin_family = AF_INET;
	channel.sin_addr.s_addr = htonl(INADDR_ANY);
	channel.sin_port = htons(port);

	if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
					&opt, sizeof(opt)) < 0)
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	if (bind(server_socket, (struct sockaddr *) &channel, sizeof(channel)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_socket, backlog) < 0)
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}

	return server_socket;
}

static void accept_clients(int server_socket)
{
	char	buf[256];
	int		client_socket;
	int		count;
	int		bytes_read;
	int		out_msg_len;

	count = 0;
	while (1)
	{
		client_socket = accept(server_socket, 0, 0);
		if (client_socket < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		count++;
		printf("Client accepted %d\n", count);
		bytes_read = read(client_socket, buf, sizeof(buf) / sizeof(buf[0]));
		if (bytes_read < 0)
		{
			perror("read");
			exit(EXIT_FAILURE);
		}
		sleep(3);
		out_msg_len = snprintf(buf, sizeof(buf)/sizeof(buf[0]),
			"#%d your message length is %d\n", count, bytes_read);
		write(client_socket, buf, out_msg_len);
		close(client_socket);
	}
}
