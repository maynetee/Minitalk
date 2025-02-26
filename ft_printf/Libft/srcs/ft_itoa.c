/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 14:17:11 by mteichma          #+#    #+#             */
/*   Updated: 2024/11/21 22:52:14 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_digits(long n)
{
	int	count;

	if (n == 0)
		return (1);
	count = 0;
	while (n != 0)
	{
		count++;
		n /= 10;
	}
	return (count);
}

static void	fill_number(char *str, long n, int index)
{
	if (n == 0)
	{
		str[index] = '0';
		return ;
	}
	while (n > 0)
	{
		str[index] = (n % 10) + '0';
		n /= 10;
		index--;
	}
}

char	*ft_itoa(int n)
{
	char	*str;
	int		len;
	long	num;

	num = (long)n;
	if (num < 0)
		num = -num;
	len = count_digits(num);
	if (n < 0)
		len++;
	str = (char *)malloc((len + 1) * sizeof(char));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (n < 0)
		str[0] = '-';
	fill_number(str, num, len - 1);
	return (str);
}
