/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_pointer.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/14 21:28:36 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/01 17:03:41 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_pointer_hex(unsigned long addr, char *base)
{
	char	hex[16];
	int		i;
	int		count;

	i = 0;
	while (addr > 0)
	{
		hex[i] = base[addr % 16];
		addr /= 16;
		i++;
	}
	count = i;
	while (i > 0)
	{
		i--;
		ft_putchar_fd(hex[i], 1);
	}
	return (count);
}

int	print_pointer(void *ptr)
{
	int	count;

	if (ptr == NULL)
	{
		ft_putstr_fd("(nil)", 1);
		return (5);
	}
	ft_putstr_fd("0x", 1);
	count = 2;
	count += print_pointer_hex((unsigned long)ptr, "0123456789abcdef");
	return (count);
}
