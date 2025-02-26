/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:09:24 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/26 11:09:25 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static int	g_received = 0;

static void	handle_confirmation(int sig)
{
	(void)sig;
	g_received = 1;
}

static void	send_bit(int pid, int bit)
{
	g_received = 0;
	if (bit == 0)
		kill(pid, SIGUSR1);
	else
		kill(pid, SIGUSR2);
	while (!g_received)
		usleep(10);
}

static void	send_char(int pid, unsigned char c)
{
	int	i;
	int	bit;

	i = 0;
	while (i < 8)
	{
		bit = (c >> i) & 1;
		send_bit(pid, bit);
		i++;
	}
}

static void	send_string(int pid, char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		send_char(pid, (unsigned char)str[i]);
		i++;
	}
	send_char(pid, '\0');
	ft_printf("Message sent and confirmed!\n");
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
	pid = atoi(argv[1]);
	if (pid <= 0)
	{
		ft_printf("Error: Invalid PID\n");
		return (1);
	}
	sa.sa_handler = handle_confirmation;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	send_string(pid, argv[2]);
	return (0);
}
