#ifndef CHIP_GUESS_COMMON_H
#define CHIP_GUESS_COMMON_H

enum {MIN_NUMBER = 1, MAX_NUMBER = 100};
enum {NUMBER_ERROR, NUMBER_EQUAL, NUMBER_LESS, NUMBER_GREATER};

int		rand_number(int min, int max);
void	send_number(int socket, int number);
int		recv_number(int socket);

#endif
