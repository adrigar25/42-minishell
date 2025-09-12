int	ft_putchar_fd(char c, int fd)
{
	if (write(fd, &c, 1) == -1)
		return (-1);
	return (0);
}