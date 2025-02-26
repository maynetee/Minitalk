/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_dispatch.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:13:12 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/01 17:04:11 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	dispatch_format(char c, va_list args)
{
	if (c == 'c')
		return (print_char(va_arg(args, int)));
	if (c == 's')
		return (print_string(va_arg(args, char *)));
	if (c == 'p')
		return (print_pointer(va_arg(args, void *)));
	if (c == 'd' || c == 'i')
		return (print_int(va_arg(args, int)));
	if (c == 'u')
		return (print_uint(va_arg(args, unsigned int)));
	if (c == 'x')
		return (print_hex_lower(va_arg(args, unsigned int)));
	if (c == 'X')
		return (print_hex_upper(va_arg(args, unsigned int)));
	if (c == '%')
		return (print_percent());
	return (0);
}
