/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_signal.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: radubos <radubos@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 09:36:08 by moritzknoll       #+#    #+#             */
/*   Updated: 2025/06/25 22:30:40 by radubos          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <signal.h>

/* 0: parent SIGINT, 1: parent SIGQUIT,
   2: heredoc SIGINT, 3: heredoc SIGQUIT */
static struct sigaction g_old_signals[4];

/* Parent prompt SIGINT handler */
static void sigint_handler(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    g_exit_status = 130;
    rl_replace_line("", 0);
    rl_on_new_line();
    rl_redisplay();
}

/* Setup signals for the interactive prompt, save old actions */
void setup_parent_signals(void)
{
    struct sigaction sa;

    /* save old parent handlers */
    sigaction(SIGINT,  NULL, &g_old_signals[0]);
    sigaction(SIGQUIT, NULL, &g_old_signals[1]);

    sa.sa_handler = sigint_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;               // no SA_RESTART
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

/* Restore saved prompt handlers */
void restore_parent_signals(void)
{
    sigaction(SIGINT,  &g_old_signals[0], NULL);
    sigaction(SIGQUIT, &g_old_signals[1], NULL);
}

/* SIGINT handler during heredoc */
static void sigint_heredoc(int sig)
{
    (void)sig;
    write(STDOUT_FILENO, "\n", 1);
    rl_on_new_line();
    rl_replace_line("", 0);
    rl_redisplay();
    g_exit_status = 130;
}

void setup_heredoc_signals(void)
{
    struct sigaction sa;

    /* save old heredoc handlers */
    sigaction(SIGINT,  NULL, &g_old_signals[2]);
    sigaction(SIGQUIT, NULL, &g_old_signals[3]);

    sa.sa_handler = sigint_heredoc;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;            // no SA_RESTART: interrupt readline
    sigaction(SIGINT, &sa, NULL);

    sa.sa_handler = SIG_IGN;
    sigaction(SIGQUIT, &sa, NULL);
}

void restore_heredoc_signals(void)
{
    sigaction(SIGINT,  &g_old_signals[2], NULL);
    sigaction(SIGQUIT, &g_old_signals[3], NULL);
}

/* In child before exec: default behavior */
void setup_child_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_DFL;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}

/* Parent ignores signals during external execution */
void ignore_signals(void)
{
    struct sigaction sa;

    sa.sa_handler = SIG_IGN;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGQUIT, &sa, NULL);
}