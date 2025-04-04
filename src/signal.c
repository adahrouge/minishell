/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:39:51 by abouraad          #+#    #+#             */
/*   Updated: 2025/04/04 15:57:53 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#define _POSIX_C_SOURCE 200809L

// "Create a global object (global_signal)
// that has all the fields defined in t_sig."
// t_sig global_signal;
// void init_global_signal(void)
// {
//     global_signal.pid = 0;
//     global_signal.sigint = 0;
//     global_signal.sigquit = 0;
// }
// void sigint_handler(int signum)
// {
//     (void)signum;
//     if (global_signal.pid == 0)
//     {
//         ft_putstr_fd("\b\b  ", STDERR);
// 		ft_putstr_fd("\n", STDERR);
// 		ft_putstr_fd("minishell >", STDERR);
//         global_signal.sigint = 1;        
//     }
//     else
//     {
//         kill(global_signal.pid, SIGINT);
//         global_signal.sigint = 130;
//     }
// }

// void setup_signal_handlers(void)
// {
//     struct sigaction sa;

//     sa.sa_handler = sigint_handler;
//     sigemptyset(&sa.sa_mask);
//     sa.sa_flags = SA_RESTART; 
//     sigaction(SIGINT, &sa, NULL);

//     sa.sa_handler = SIG_IGN; // Ignore SIGQUIT
//     sigaction(SIGQUIT, &sa, NULL);
// }

// void sigint_handler(int signum)
// {
//     (void)signum;
//     write(1, "\n", 1);             // Move to a new line
//     rl_on_new_line();              // Prepare readline for a new line
//     rl_replace_line("", 0);        // Clear the current line input
//     rl_redisplay();                // Redisplay the prompt
// }
// void sigquit_handler(int signum)
// {
//     (void)signum;
// }

void	sigint_handler(int signum)
{
	(void)signum;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigquit_handler(int signum)
{
	(void)signum;
}

void	setup_signal_handlers(void)
{
	struct sigaction sa;

	sa.sa_handler = sigint_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
}
