/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:09:24 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/26 11:53:59 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_received = 0;

static void	handle_confirmation(int sig)
{
	(void)sig;
	g_received = 1;
}

static int	send_bit(int pid, int bit)
{
	int	result;
	int	retry;

	retry = 1000;
	g_received = 0;
	if (bit == 0)
		result = kill(pid, SIGUSR1);
	else
		result = kill(pid, SIGUSR2);
	if (result == -1)
		return (-1);
	while (!g_received && retry > 0)
	{
		usleep(250);
		retry--;
	}
	if (!g_received)
		return (-1);
	return (0);
}

static int	send_char(int pid, unsigned char c)
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
		if (send_char(pid, (unsigned char)str[i]) == -1)
			return (-1);
		i++;
	}
	if (send_char(pid, '\0') == -1)
		return (-1);
	usleep(100);
	ft_printf("Message sent and confirmed!\n");
	return (0);
}

int	main(int argc, char **argv)
{
	int					pid;
	struct sigaction	sa;

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
	sa.sa_handler = handle_confirmation;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	if (send_string(pid, argv[2]) == -1)
	{
		ft_printf("Error: Failed to send message to PID %d\n", pid);
		return (1);
	}
	return (0);
}
