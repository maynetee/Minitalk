/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mteichma <mteichma@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 11:09:15 by mteichma          #+#    #+#             */
/*   Updated: 2025/02/26 11:52:02 by mteichma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

static void	send_confirmation(int client_pid)
{
	usleep(500);
	kill(client_pid, SIGUSR1);
}

static void	output_current_char(int *bit_index, unsigned char *current_char,
		int *client_pid)
{
	if (*current_char == '\0')
	{
		write(1, "\n", 1);
		*client_pid = 0;
	}
	else
		write(1, current_char, 1);
	*bit_index = 0;
	*current_char = 0;
}

static void	handle_signal(int sig, siginfo_t *info, void *context)
{
	static int				bit_index = 0;
	static unsigned char	current_char = 0;
	static int				client_pid = 0;

	(void)context;
	if (client_pid == 0)
		client_pid = info->si_pid;
	if (sig == SIGUSR2)
		current_char |= (1 << bit_index);
	bit_index++;
	if (client_pid != 0)
		send_confirmation(client_pid);
	if (bit_index == 8)
		output_current_char(&bit_index, &current_char, &client_pid);
}

int	main(void)
{
	struct sigaction	sa;

	ft_printf("Server PID: %d\n", getpid());
	sa.sa_sigaction = handle_signal;
	sa.sa_flags = SA_SIGINFO;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGUSR1, &sa, NULL);
	sigaction(SIGUSR2, &sa, NULL);
	while (1)
		pause();
	return (0);
}
