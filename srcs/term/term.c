/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   term.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/16 13:20:36 by chamada           #+#    #+#             */
/*   Updated: 2020/12/13 01:50:28 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

static int	ft_isatty(int fd)
{
	struct stat	st;
	int			ret;

	ret = 0;
	if (fstat(fd, &st))
		ft_dprintf(STDERR_FILENO, "Could not stat fd %d: %s\n",
			fd, strerror(errno));
	else if (S_ISCHR(st.st_mode))
		ret = 1;
	return (ret);
}

t_term_err	term_init(t_env **env, const char *cwd, bool is_login)
{
	t_term_err	status;

	(void)is_login;
	if ((g_term.line = line_new(0)))
	{
		status = TERM_EOK;
		g_term.is_interactive =
			ft_isatty(STDIN_FILENO) && ft_isatty(STDERR_FILENO);
		if (g_term.is_interactive
		&& (status = term_init_env(env, cwd)) != TERM_EOK)
			line_clear(&g_term.line);
	}
	else
		status = TERM_EALLOC;
	return (status);
}

void		term_destroy(void)
{
	if (g_term.caps.hist.next != g_term.line)
		line_clear(&g_term.caps.hist.next);
	if (g_term.caps.hist.last != g_term.line)
		line_clear(&g_term.line);
	line_clear(&g_term.msg);
	if (g_term.is_interactive)
		write(STDERR_FILENO, TERM_EXIT, sizeof(TERM_EXIT) - 1);
}

/*
**	Prompt the user of an interactive terminal.
*/

t_term_err	term_prompt(const char **dst)
{
	t_term_err	status;

	if (g_term.is_interactive && g_term.msg->len
	&& write(STDERR_FILENO, g_term.msg->data, g_term.msg->len) == -1)
		status = TERM_EWRITE;
	else
		status = term_read();
	if (dst)
		*dst = g_term.line->data;
	return (status);
}
