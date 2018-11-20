#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>

static int create_client_socket(const char *address, int port);
static void	interact_with_server(int client_socket);

int main(int argc, char *argv[]) 
{
	int			client_socket;

	client_socket = create_client_socket("127.0.0.1", 8000);
	interact_with_server(client_socket);
	close(client_socket);

	return 0; 
}

static int create_client_socket(const char *address, int port)
{
	struct sockaddr_in	server_address = {};
	int					client_socket;

	if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);

	if (inet_pton(AF_INET, address, &server_address.sin_addr) <= 0)
	{
		perror("inet_pton");
		exit(EXIT_FAILURE);
	}
	if (connect(client_socket, (struct sockaddr *) &server_address,
		sizeof(server_address)) < 0)
	{
		perror("connect");
		exit(EXIT_FAILURE);
	}

	return client_socket;
}

static void	interact_with_server(int client_socket)
{
	char		inbuf[256];
	const char	outbuf[] = "Get me some information";
	int			bytes_read;

	if (write(client_socket, outbuf, sizeof(outbuf)/sizeof(outbuf[0]) - 1) < 0)
	{
		perror("write");
		exit(EXIT_FAILURE);
	}
	bytes_read = read(client_socket, inbuf, sizeof(inbuf) / sizeof(inbuf[0]));
	if (bytes_read < 0)
	{
		perror("read");
		exit(EXIT_FAILURE);
	}
	inbuf[bytes_read] = '\0';
	printf("%s\n", inbuf);
}
