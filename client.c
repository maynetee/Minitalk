/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:09:21 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/26 11:52:19 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	send_bit(int pid, int bit)
{
	int	result;

	if (bit == 0)
		result = kill(pid, SIGUSR1);
	else
		result = kill(pid, SIGUSR2);
	if (result == -1)
		return (-1);
	usleep(300);
	return (0);
}

static int	send_char(int pid, char c)
{
	int	i;
	int	bit;

	i = 0;
	while (i < 8)
	{
		bit = (c >> i) & 1;
		if (send_bit(pid, bit) == -1)
			return (-1);
		i++;
	}
	return (0);
}

static int	send_string(int pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (send_char(pid, str[i]) == -1)
			return (-1);
		i++;
	}
	if (send_char(pid, '\0') == -1)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	int	pid;

	if (argc != 3)
	{
		ft_printf("Usage: %s <server_pid> <message>\n", argv[0]);
		return (1);
	}
	pid = ft_atoi(argv[1]);
	if (pid <= 0)
	{
		ft_printf("Error: Invalid PID\n");
		return (1);
	}
	if (send_string(pid, argv[2]) == -1)
	{
		ft_printf("Error: Failed to send message to PID %d\n", pid);
		return (1);
	}
	return (0);
}
