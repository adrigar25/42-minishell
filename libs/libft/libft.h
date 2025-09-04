/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adriescr <adriescr@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/03 13:22:52 by adriescr          #+#    #+#             */
/*   Updated: 2025/09/04 17:32:15 by adriescr         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define BUFFER_SIZE 1024
# define RESET_COLOR "\033[0m"

// Functions lib
int				ft_putstr_error(const char *str);
int				ft_putstr(const char *str);
int				ft_putchar(char c);
int				ft_putchar_error(char c);
int				ft_is_digit(unsigned char c);
int				ft_is_number(const char *str);
int				ft_atoi(const char *str);
char			**ft_split(char const *s, char c);
void			*ft_memcpy(void *dst, const void *src, size_t n);
size_t			ft_count_words(const char *s, char c);
size_t			ft_strlen(const char *s);
void			ft_putnbr(int n);
char			*ft_strjoin(char *s1, char *s2);
char			*ft_strdup(const char *s);
char			*ft_substr(char *s, unsigned int start, size_t len);
char			*ft_strchr(const char *s, int c);
int				ft_strcmp(const char *s1, const char *s2);
char			*ft_strstr(const char *haystack, const char *needle);
char			*ft_strcpy(char *dest, const char *src);
void			ft_skip_whitespace(const char *cmd, int *i);
int				ft_isspace(int c);

// Especial functions
unsigned int	ft_simple_rand(unsigned int *seed);
// Get Next Line
char			*ft_get_next_line(int fd);
// File utils
int				ft_close_file(int fd);
int				ft_open_file_read(const char *filename);
int				ft_open_file_write(const char *filename);
int				ft_print_file(char *filename, char *color);

#endif
