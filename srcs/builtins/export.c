/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/13 08:19:52 by pablo             #+#    #+#             */
/*   Updated: 2020/12/12 22:25:51 by pablo            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <execution/execution.h>

static bool	print_env(int fd, t_env *env)
{
	while (env)
	{
		if (env->exported)
		{
			if (env->key[env->key_length] == ENV_ASGN)
				ft_dprintf(fd, "declare -x %.*s=\"%s\"\n",
					(int)env->key_length, env->key, env->key + \
						env->key_length + 1);
			else
				ft_dprintf(fd, "declare -x %s\n", env->key);
		}
		env = env->next;
	}
	return (SUCCESS);
}

static void	print_error(t_exec *args, int *ret, int i)
{
	ft_dprintf(STDERR_FILENO, "%s: export: `%s': not a valid identifier\n",
			args->session->name, args->av[i]);
	*ret = STD_ERROR;
}

int			b_export(t_exec *args)
{
	int		ret;
	int		i;
	char	assign_st;

	ret = SUCCESS;
	i = 0;
	if (args->ac == 1)
		return (print_env(args->fds[FDS_STDOUT], args->session->env));
	while (++i < args->ac)
	{
		if (env_key_len(args->av[i], true))
		{
			if ((assign_st = env_assign(&args->session->env, \
					args->av[i], true, true)) == 0)
				env_set(&args->session->env, args->av[i], NULL, true);
			else if (assign_st == -1)
				return (STD_ERROR);
		}
		else
			print_error(args, &ret, i);
	}
	return (ret);
}
