/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr >         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:09:21 by mteichma          #+#    #+#             */
/*   Updated: 2025/03/06 17:18:45 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

int	g_ack = 0;

static void	handle_ack(int sig)
{
	(void)sig;
	g_ack = 1;
}

static int	send_bit(int pid, int bit)
{
	int	timeout;

	g_ack = 0;
	if ((bit == 1 && kill(pid, SIGUSR2) == -1) || (bit == 0 && kill(pid,
				SIGUSR1) == -1))
	{
		ft_printf("Error: invalid PID\n");
		return (-1);
	}
	usleep(70);
	timeout = 5000;
	while (!g_ack && timeout > 0)
	{
		usleep(1000);
		timeout--;
	}
	if (g_ack)
		return (0);
	return (-1);
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

static int	send_string(int pid, const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (send_char(pid, (unsigned char)str[i]) == -1)
			return (-1);
		i++;
	}
	if (send_char(pid, 0) == -1)
		return (-1);
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
	sa.sa_handler = handle_ack;
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
