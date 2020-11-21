/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   job_control.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 19:39:58 by pablo             #+#    #+#             */
/*   Updated: 2020/11/20 23:58:06 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <process.h>
#include <libft.h>
#include <sys/wait.h>
#include <signal.h>

// for the momment keep them

void		update_background(t_session* session, t_process **target)
{
	//ft_dprintf(2, "[UPDATE V2--1]ACTIVE PROCESSES: %p\n", (*target));
	ft_dprintf(2, "[WAIING...][pid=\'%d\']\n", (*target)->pid);
	while (waitpid((*target)->pid, &(*target)->wstatus, WUNTRACED) <= 0)
		;
	// exited and not stopped
	// HERE CAN REMOVE FLAGS IN ALL CASES AND ADD IT IF NECESARRY
	if (WIFEXITED((*target)->wstatus) || !WIFSTOPPED((*target)->wstatus))
	{
		ft_dprintf(2, "[PROCESS EXITS]\n");
		(*target)->flags &= ~STOPPED;
	}
	// stopped
	else
	{
		ft_dprintf(2, "[PROCESS DOESN'T EXIT]\n");
		(*target)->flags |= STOPPED;
		if (is_leader(session, *target))
			update_session_history(session, *target);
	}
	//ft_dprintf(2, "[UPDATE V2--2]ACTIVE PROCESSES: %p\n", (*target));
	//ft_dprintf(2, "------> %d\n", (*target)->flags);
}

bool            update_session_history(t_session *session, t_process *update)
{
    t_process*  cp_update;
	t_process*	fill;

	ft_dprintf(2, "[UPDATE SESSION HISTORY (make a copy) ][\'%p\']\n", update);
    if (!(cp_update = process_new(update->pid, update->wstatus, update->data)))
		return (false);
	fill = session->history;
	session->history = cp_update;
	cp_update->next = fill;
	if (fill)
		fill->prev = cp_update;
	return (true);
}

t_process**		background_find(t_process* target, const char* search_type, t_group* group)
{
	const char*	modes[2] = { "PID", "STA" };
	int 		i;

	i = 0;
	while (group->active_processes != group->nil)
	{
		while (i < 2 && ft_strncmp(modes[i], search_type, 3))
			i++;
		if (!i && target->pid == group->active_processes->pid)
			return (&group->active_processes);
		else if (i && target->wstatus == group->active_processes->wstatus)
			return (&group->active_processes);
		group->active_processes = group->active_processes->next;
	}
	return (NULL);
}

bool		is_active_group(t_group* target)
{
	t_process*	process;

	process = target->nil->next;
	ft_dprintf(2, "[IS ACTIVE GROUP (leader): %p]\n", process);
	while (process != target->nil)
	{
		if (process->flags & (BACKGROUD | STOPPED))
			return (true);
		process = process->next;
	}
	return (false);
}

pid_t			get_process_leader_pid(t_group* nil, t_process* target)
{
	t_group*	groups;

	groups = nil->next;
	while (groups && groups != nil)
	{
		if (background_find(target, "PID", groups))
			return (groups->nil->next->pid);
		groups = groups->next;
	}
	return (0);
}

size_t			get_background_index(t_group* nil, t_process* target)
{
	size_t		index;
	t_process*	fill;
	t_group*	groups;

	index = 0;
	groups = nil->prev;
	while (groups != nil && (index++))
	{
		fill = groups->nil->prev;
		while (fill != groups->nil)
		{
			if (fill == target)
				return (index);
			fill = fill->prev;
		}
		groups = groups->prev;
	}
	return (index);
}

/*

void			force_exit_background(t_session* session)
{
	t_process**	fill;

	while (session->groups != session->nil)
	{
		while (session->groups->active_processes != session->groups->nil)
		{
			// TO DO: for SIGTTIN SIGTTOU -> SIGHUB
			kill(session->groups->active_processes->pid, SIGCONT);
			// wait for the process to exit and then iterate
			while (waitpid(session->groups->active_processes->pid, &session->groups->active_processes->wstatus, 0) <= 0)
				;
			if (WIFEXITED(session->groups->active_processes->wstatus))
			{
				fill = &session->groups->active_processes;
				session->groups->active_processes = session->groups->active_processes->next;
				remove_process(fill);
			}
			session->groups->active_processes = session->groups->active_processes->next;
		}
		session->groups = session->groups->next;
	}
}
*/

bool			is_leader(t_session* session, t_process* target)
{
	t_group*	groups;

	if (!target)
		return (false);
	groups = session->groups;
	while (groups != session->nil && groups->nil && groups->nil->next)
	{
		if (groups->nil->next && groups->nil->next->pid == target->pid)
			return (true);
		groups = groups->next;
	}
	return (false);
}