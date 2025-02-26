/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_num.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 18:19:46 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/01 17:03:27 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	print_int(int n)
{
	char	*str;
	int		len;

	str = ft_itoa(n);
	if (!str)
		return (0);
	ft_putstr_fd(str, 1);
	len = ft_strlen(str);
	free(str);
	return (len);
}

int	print_uint(unsigned int n)
{
	int	len;

	if (n == 0)
	{
		ft_putchar_fd('0', 1);
		return (1);
	}
	len = 0;
	if (n >= 10)
		len += print_uint(n / 10);
	ft_putchar_fd((n % 10) + '0', 1);
	return (len + 1);
}
