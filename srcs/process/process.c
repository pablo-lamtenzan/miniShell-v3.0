/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 07:51:17 by pablo             #+#    #+#             */
/*   Updated: 2020/11/14 10:33:17 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/wait.h>
#include <signal.h>

#include <execution.h>
#include <signals.h>

static int	handle_wstatus(int wstatus, char*const* av, t_process* suspended)
{
	int		i;

	i = -1;
	if (WIFEXITED(wstatus))
		return (WEXITSTATUS(wstatus));
	if (WIFSIGNALED(wstatus) && (wstatus = WTERMSIG(wstatus)))
	{
			print_signals(wstatus, (const char**)av, suspended);
			wstatus += 128;
	}
	return (wstatus);
}

static void		process_push_back(t_process** root, t_process* n)
{
	t_process*	tmp;

	tmp = *root;
	while (tmp->data)
		tmp = (t_process*)tmp->data;
	tmp->data = n;
}

// use for print signals
size_t			suspended_process_nb(t_process* suspended)
{
	t_process*	tmp;
	size_t		nb;

	tmp = suspended;
	nb = 1;
	while (tmp && (nb++))
		tmp = (t_process*)tmp->data;
	return (nb - 1);
}

bool        	wait_processes(t_term* term)
{
    int     	i;
	t_process	*suspended;

    i = 0;
    while (++i < term->processes[MANAGE].pid)
    {
        while (waitpid(term->processes[i].pid, &term->processes[i].wstatus, 0) <= 0)
            ;
		if (term->processes[i].wstatus == SIGSTOP || term->processes[i].wstatus == SIGTSTP \
				|| term->processes[i].wstatus == SIGTTIN || term->processes[i].wstatus == SIGTTOU)
		{
			if (!(suspended = malloc(sizeof(t_process))))
				return (false);
			*suspended = (t_process){.pid=term->processes[i].pid, .wstatus=term->st - 128, .data=NULL};
			if (term->suspended_processes)
				process_push_back(&term->suspended_processes, suspended);
		}
        term->st = handle_wstatus(&term->processes[i].wstatus, (char*const*)&term->processes[i].data, term->suspended_processes);
    }
	return (true);
rt