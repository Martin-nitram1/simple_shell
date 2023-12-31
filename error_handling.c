#include "shell.h"

/**
 * _eputs - prints an input strings
 * @str: the string to print
 *
 * Return: void
 **/
void _eputs(char *str)
{
	int j = 0;

	if (!str)
		return;

	while (str[j] != '\0')
	{
		_eputchar(str[j]);
		j++;
	}
}
/**
 * _eputchar - writes char to stderr
 * @c: char to print
 *
 * Return: 1 on success, -1 on error and errno
 * set appropriately
 **/
int _eputchar(char c)
{
	static int j;
	static char buf[BUFFER_SIZE_WRITE];

	if (c == FLUSH_BUFFER || j >= BUFFER_SIZE_WRITE)
	{
		write(2, buf, j);
		j = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[j++] = c;
	return (1);
}
/**
 * _putfd - write char to fd
 * @c: char to print
 * @fd: file descriptor
 *
 * Return: 1 on success, -1 on error and errno set
 **/
int _putfd(char c, int fd)
{
	static int j;
	static char buf[BUFFER_SIZE_WRITE];

	if (c == FLUSH_BUFFER || j >= BUFFER_SIZE_WRITE)
	{
		write(fd, buf, j);
		j = 0;
	}
	if (c != FLUSH_BUFFER)
		buf[j++] = c;
	return (1);
}
/**
 * _putsfd - print to input string
 * @str: string to print
 * @fd: file descriptor
 *
 * Return: num of chars
 **/
int _putsfd(char *str, int fd)
{
	int j = 0;

	if (!str)
		return (0);

	while (*str)
	{
		j += _putfd(*str++, fd);
	}
	return (j);
}
