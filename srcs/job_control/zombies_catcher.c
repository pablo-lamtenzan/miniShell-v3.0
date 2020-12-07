/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zombies_catcher.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/28 01:45:31 by pablo             #+#    #+#             */
/*   Updated: 2020/12/07 10:36:25 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <job_control.h>

void				handle_wstatus(t_group **group)
{
	if (WIFEXITED((*group)->active_processes->wstatus))
	{
		(*group)->active_processes->flags |= (EXITED | NO_DELETE);
		(*group)->active_processes->ret = \
			WEXITSTATUS((*group)->active_processes->wstatus);
		//ft_dprintf(2, "\n[ZOMBIE CATCHER][GROUP %p EXITED!]\n", *group);
		deadzombie_push_back(deadzombie_new((*group)->active_processes));
	}
	
	else if (WIFSTOPPED((*group)->active_processes->wstatus))
	{
		g_session.zombies->exited = false;
		(*group)->active_processes->flags |= STOPPED;
		(*group)->active_processes->ret = SIGNAL_BASE + \
			WSTOPSIG((*group)->active_processes->wstatus);
		//ft_dprintf(2, "\n[ZOMBIE CATCHER][GROUP %p STOPPED!]\n", *group);
	}
	else if (WIFSIGNALED((*group)->active_processes->wstatus))
	{
		g_session.st = SIGNAL_BASE + \
			WTERMSIG((*group)->active_processes->wstatus);
		//ft_dprintf(2, "\n[ZOMBIE CATCHER][GROUP %p SIGNALED!]\n", *group);
	}
}

void				catch_group(t_group **group)
{
	t_process		*leader;

	leader = (*group)->active_processes;
	while ((*group)->active_processes != (*group)->nil)
	{
		if ((*group)->active_processes->flags & BACKGROUND)
		{
			//(*group)->active_processes->wstatus = 0;
			int test;
			if ((test = waitpid((*group)->active_processes->pid, \
				&(*group)->active_processes->wstatus, WNOHANG | WUNTRACED) > 0))
			{
				g_session.zombies->exited = true;
				(*group)->active_processes->flags &= ~BACKGROUND;
				handle_wstatus(group);
			}
		}
		(*group)->active_processes = (*group)->active_processes->next;
	}
	(*group)->active_processes = leader;
}

void				zombies_catcher(int signal)
{
	t_background	*first;

	(void)signal;
	first = g_session.zombies;
	while (g_session.zombies)
	{
		if (g_session.zombies->background_group)
			catch_group(&g_session.zombies->background_group);
		g_session.zombies = g_session.zombies->next;
	}
	g_session.zombies = first;
}
