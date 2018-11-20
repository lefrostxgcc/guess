#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <guess_common.h>

static int create_server_socket(int port, int backlog);
static void accept_clients(int server_socket);
static int check_client_number(int socket, int guessed_number);

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
	int			client_socket;
	int			guessed_number;
	int			client_number;
	int			result;

	guessed_number = rand_number(MIN_NUMBER, MAX_NUMBER);
	printf("Guess number in [%d, %d] is %d\n", MIN_NUMBER, MAX_NUMBER,
		guessed_number);
	while (1)
	{
		client_socket = accept(server_socket, 0, 0);
		if (client_socket < 0)
		{
			perror("accept");
			exit(EXIT_FAILURE);
		}
		result = check_client_number(client_socket, guessed_number);
		if (result == NUMBER_EQUAL)
			printf("Client guess number!\n");
		send_number(client_socket, result);
		close(client_socket);
	}
}

static int check_client_number(int socket, int guessed_number)
{
	int			client_number;

	client_number = recv_number(socket);
	if (client_number == 0)
		return NUMBER_ERROR;
	else if (client_number < guessed_number)
		return NUMBER_GREATER;
	else if (client_number > guessed_number)
		return NUMBER_LESS;
	else
		return NUMBER_EQUAL;
}
