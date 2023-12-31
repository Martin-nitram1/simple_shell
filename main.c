#include "shell.h"

/**
 * main - entry point
 * @ac: argument counter
 * @av: argument vector
 *
 * Return: 0 if success, 0 otherwise
 **/
int main(int ac, char **av)
{
	info_t info[] = { COMMAND_INFO_INITIALIZER };
	int fd = 2;

	fd = fd + 3;

	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_eputs(av[0]);
				_eputs("failed!");
				_eputs(av[1]);
				_eputchar('\n');
				_eputchar(FLUSH_BUFFER);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->read_file = fd;
	}
	populate_environ(info);
	read_hist(info);
	custom_shell(info, av);
	return (EXIT_SUCCESS);
}
