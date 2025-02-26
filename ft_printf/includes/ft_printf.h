/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:41:55 by mteichma          #+#    #+#             */
/*   Updated: 2024/12/17 19:05:24 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include <unistd.h>

int	ft_printf(const char *format, ...);
int	print_format(const char *format, va_list args);
int	dispatch_format(char c, va_list args);
int	print_char(int c);
int	print_string(char *s);
int	print_pointer(void *ptr);
int	print_int(int n);
int	print_uint(unsigned int n);
int	print_hex_lower(unsigned int n);
int	print_hex_upper(unsigned int n);
int	print_percent(void);

#endif
