/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adahroug <adahroug@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/31 12:39:51 by abouraad          #+#    #+#             */
/*   Updated: 2024/11/08 20:24:05 by adahroug         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include <signal.h>
#include <readline/readline.h>
#include "minishell.h"

void sigint_handler(int signum)
{
    (void)signum;
    write(1, "\n", 1);             // Move to a new line
    rl_on_new_line();              // Prepare readline for a new line
    rl_replace_line("", 0);        // Clear the current line input
    rl_redisplay();                // Redisplay the prompt
}
void sigquit_handler(int signum)
{
    (void)signum;
}
void setup_signal_handlers(void)
{
    struct sigaction sa;

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0; // Ensure interrupted system calls are restarted
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN; // Ignore SIGQUIT
    sigaction(SIGQUIT, &sa, NULL);
}
