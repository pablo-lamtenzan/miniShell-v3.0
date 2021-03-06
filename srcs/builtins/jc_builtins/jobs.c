/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 12:03:23 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 21:59:55 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>
#include <signals_print.h>
#include <job_control/jc_builtins.h>
#include <job_control/jobspec_parser.h>
#include <job_control/conditions.h>
#include <job_control/utils.h>

static void		print_all_leaders(int fd, int flags)
{
	t_group		*remember;

	remember = g_session.groups;
	g_session.groups = g_session.nil->prev;
	while (g_session.groups != g_session.nil->next)
	{
		if (!(flags & 2 && group_condition(g_session.groups, is_not_running))
				&& !(flags & 4 \
				&& group_condition(g_session.groups, is_not_stopped))
				&& !group_condition(g_session.groups, is_exited))
			print_signal(fd, g_session.groups->nil->next, STANDART);
		g_session.groups = g_session.groups->prev;
	}
	g_session.groups = remember;
}

static void		print_all_groups(int fd, int flags)
{
	t_group		*remember;

	remember = g_session.groups;
	g_session.groups = g_session.nil->prev;
	while (g_session.groups != g_session.nil->next)
	{
		if (!group_condition(g_session.groups, is_exited))
			print_group(fd, g_session.groups->nil->next, flags, remember);
		g_session.groups = g_session.groups->prev;
	}
	g_session.groups = remember;
}

static int		jobs_init_exeptions(t_exec *args, int *flags, int *nb)
{
	if ((*flags = parse_flags(args->ac, &args->av[1], "nrsl", nb)) < 0 \
			&& args->av[*nb + 1][0] == '-')
	{
		ft_dprintf(STDERR_FILENO, "%s: jobs: %s: invalid option\n%s%s", \
			g_session.name, args->av[1], "jobs: usage: jobs: [-lnprs]", \
			"[jobspec ...] or jobs -x command [args]\n");
		return (CMD_BAD_USE);
	}
	if (session_empty() || g_session.groups->next == g_session.nil)
	{
		if (args->ac > 1 && *flags < 0)
		{
			ft_dprintf(STDERR_FILENO, "%s: jobs: %s: no such job\n", \
				g_session.name, args->av[args->ac]);
			return (STD_ERROR);
		}
		return (SUCCESS);
	}
	return (42);
}

static int		jobs_jobspec(t_exec *args, int nb, int flags)
{
	int			i;
	t_process	**target;

	i = -1;
	while (++i < args->ac - nb - 1)
	{
		if (!(target = jobspec_parser(args->ac, &args->av[nb + i], ignore_pid)))
		{
			ft_dprintf(STDERR_FILENO, "%s: jobs: %s: no such job\n", \
				g_session.name, args->av[nb + i + 1]);
			return (STD_ERROR);
		}
		if (flags & 8)
			print_group(args->fds[1], *target, flags, g_session.groups);
		else if (!(flags & 2 && group_condition(g_session.groups, \
				is_not_running)) && !(flags & 4 \
				&& group_condition(g_session.groups, is_not_stopped))
				&& !group_condition(g_session.groups, is_exited))
			print_signal(args->fds[1], *target, STANDART);
	}
	return (SUCCESS);
}

int				b_jobs(t_exec *args)
{
	int			flags;
	int			exept;
	int			nb;

	flags = 0;
	if ((exept = jobs_init_exeptions(args, &flags, &nb)) != 42)
		return (exept);
	flags = flags < 0 ? -flags : flags;
	if (args->ac > 1 && args->ac > nb + 1)
		return (jobs_jobspec(args, nb, flags));
	if (flags & 8)
		print_all_groups(args->fds[1], flags);
	else
		print_all_leaders(args->fds[1], flags);
	return (SUCCESS);
}
