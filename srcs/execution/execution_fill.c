/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_fill.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 02:45:41 by pablo             #+#    #+#             */
/*   Updated: 2020/11/28 01:55:31 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>
#include <path.h>
#include <errors.h>
#include <errno.h>

static int	ft_fork(t_exec* info, t_term* term) 
{
	(void)term;
	int				child_st;
	t_process*		process;

	if ((child_st = fork()) > 0)
	{
		if (!(process = process_new(child_st, 0, info->av)))
			return (-1);
		process_push_back(&g_session->groups, process);
	}
	return (child_st);
}

int		execute_child(t_exec* info, t_term* term)
{
	int				ret;
	pid_t			pid;
	t_exec_status	st;

	ret = 0;
	if (!(pid = ft_fork(info, term)))
	{
		if ((st = dup_stdio(info->fds)) != SUCCESS)
			return (st);
		ret = execve(info->execution_path, info->av, info->ep);
		ft_dprintf(STDERR_FILENO, "%s: %s: execve returned '%d'!\n", term->name, info->av[0], ret);
		exit(EXIT_FAILURE);
	}
	else if (pid < 0)
		return (errno);
	return (ret);
}

t_exec_status		build_execve_args(t_exec* info, t_term* term)
{
	if (!(info->execution_path = path_get(info->av[0], env_get(term->env, "PATH", 4))))
	{
		g_session->st = CMD_NOT_FOUND;
		return (BAD_PATH);
	}
	if (!(info->ep = (char*const*)env_export(term->env)))
	{
		free(info->execution_path);
		return (RDR_BAD_ALLOC);
	}
	return (SUCCESS);
}

void		destroy_execve_args(t_exec *info)
{
	free((char **)info->ep);
	free((void*)info->execution_path);
	info->exec = NULL;
}
