#include <minishell.h>

int	ft_unset(int ac, char* *av, t_term *term)
{
	// TODO: glob env
	while (ac-- > 1)
	{
		/* if (!key_check(av[ac]))
		{
			ft_printf("%s: %s `%s' : not a valid identifier\n",
				t->name, av[0], av[ac]);
			return (1);
		} */
		term->env = map_del(term->env, av[ac]);
		//glob_env = map_del(cmd->glob_env, cmd->av[cmd->ac]);
	}
	return (0);

}
