/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_hex.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 20:32:54 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/01 17:04:36 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_hex(unsigned int n, char *base)
{
	char	buffer[20];
	int		i;
	int		digits;

	if (n == 0)
	{
		ft_putchar_fd('0', 1);
		return (1);
	}
	i = 0;
	while (n > 0)
	{
		buffer[i] = base[n % 16];
		n /= 16;
		i++;
	}
	digits = i;
	while (i > 0)
	{
		i--;
		ft_putchar_fd(buffer[i], 1);
	}
	return (digits);
}

int	print_hex_lower(unsigned int n)
{
	return (print_hex(n, "0123456789abcdef"));
}

int	print_hex_upper(unsigned int n)
{
	return (print_hex(n, "0123456789ABCDEF"));
}
