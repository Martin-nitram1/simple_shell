#include "shell.h"

/**
 * input_buf - chained commands
 * @info: param struct
 * @buf: buffer
 * @len: len var
 *
 * Return: bytes read
 **/
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
	ssize_t r = 0;
	size_t len_p = 0;

	if (!*len)
	{
		free(*buf);
		*buf = NULL;
		signal(SIGINT, sigintHandler);
#if USE_GETLINE_FLAG
		r = getline(buf, &len_p, stdin);
#else
		r = getline_input(info, buf, &len_p);
#endif
		if (r > 0)
		{
			if ((*buf)[r - 1] == '\n')
			{
				(*buf)[r - 1] = '\0';
				r--;
			}
			info->linenum_flag = 1;
			remove_comlines(*buf);
			build_hist(info, *buf, info->history_count++);
		}
	}
	return (r);
}
/**
 * read_input - get ine minus newline
 * @info: param
 *
 * Return: bytes read
 **/
ssize_t read_input(info_t *info)
{
	static char *buf;
	static size_t i, j, len;
	ssize_t r = 0;
	char **buf_p = &(info->argument), *p = NULL;

	_putchar(FLUSH_BUFFER);
	r = input_buf(info, &buf, &len);
	if (r == -1)
		return (-1);
	if (len)
	{
		j = i;
		p = buf + i;

		check_command(info, buf, &j, i, len);
		while (j < len)
		{
			if (command_chain(info, buf, &j))
				break;
			j++;
		}
		i = j + 1;
		if (i >= len)
		{
			i = len = 0;
			info->buffer_type = COMMAND_NORMAL;
		}
		*buf_p = buf;
		return (_strlen(p));
	}
	*buf_p = buf;
	return (r);
}
/**
 * read_buf - read buffer
 * @info: param
 * @buf: buffer
 * @i: size
 *
 * Return: int
 **/
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
	ssize_t rs = 0;

	if (*i)
		return (0);
	rs = read(info->read_file, buf, BUFFER_SIZE_READ);
	if (rs >= 0)
		*i = rs;
	return (rs);
}
/**
 * getline_input - get next line
 * @info: param
 * @ptr: ptr to buffer
 * @length: dsize of buff
 *
 * Return: r
 **/
int getline_input(info_t *info, char **ptr, size_t *length)
{
	static char buf[BUFFER_SIZE_READ];
	static size_t iq, len;
	size_t k;
	ssize_t rs = 0;
	ssize_t s = 0;
	char *p = NULL, *new_p = NULL, *c;

	p = *ptr;
	if (p && length)
		s = *length;
	if (iq == len)
		iq = len = 0;

	rs = read_buf(info, buf, &len);
	if (rs == -1 || (rs == 0 && len == 1))
		return (-1);
	c = _strchr(buf + iq, '\n');
	k = c ? 1 + (unsigned int)(c - buf) : len;
	new_p = _realloc(p, s, s ? s + k : k + 1);
	if (!new_p)
		return (p ? (free(p), -1) : -1);
	if (s)
		_strncat(new_p, buf + iq, k - iq);
	else
		_strncpy(new_p, buf + iq, k - iq + 1);
	s += k - iq;
	iq = k;
	p = new_p;

	if (length)
		*length = s;
	*ptr = p;
	return (s);
}
/**
 * sigintHandler - blocks ctr-c
 * @sig_num: signal
 *
 * Return: void
 **/
void sigintHandler(__attribute__((unused))int sig_num)
{
	_puts("\n");
	_puts("$ ");
	_putchar(FLUSH_BUFFER);
}
