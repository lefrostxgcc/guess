#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <guess_common.h>

static int create_client_socket(const char *address, int port);
static void	interact_with_server();
static int check_server_responce(int socket, int number, int *min, int *max);

int main(int argc, char *argv[]) 
{
	interact_with_server();

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

static void	interact_with_server()
{
	int			client_socket;
	int			client_number;
	int			server_responce;
	int			min;
	int			max;
	int			step;

	step = 1;
	min = MIN_NUMBER;
	max = MAX_NUMBER;
	client_number = rand_number(MIN_NUMBER, MAX_NUMBER);
	printf("Guess number in [%d, %d]\n", MIN_NUMBER, MAX_NUMBER);
	while (1)
	{
		client_socket = create_client_socket("127.0.0.1", 8000);
		printf("Step %d. Maybe number is %d...\t", step, client_number);
		send_number(client_socket, client_number);
		server_responce = check_server_responce(
			client_socket, client_number, &min, &max);
		printf("%s\n", status_str(server_responce));
		if (server_responce == NUMBER_EQUAL)
			break;
		client_number = (min + max) / 2;
		close(client_socket);
		sleep(2);
		step++;
	}
	printf("Guess number: %d\n", client_number);
}

static int check_server_responce(int socket, int number, int *min, int *max)
{
	int		server_responce;

	server_responce = recv_number(socket);
	switch (server_responce)
	{
		case NUMBER_LESS:
			*max = number;
			break;
		case NUMBER_GREATER:
			*min = number;
			break;
		case NUMBER_ERROR:
			fprintf(stderr, "Invalid server responce\n");
			exit(EXIT_FAILURE);
	}
	return server_responce;
}
