/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_csi_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/04 12:16:46 by: chamada          #+#    #+#             */
/*   Updated: 2020/12/09 10:51:59 by: chamada         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include <term/term.h>

t_term_err	term_read_mod_none(void)
{
	ssize_t	read_st;
	char	c;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == ';')
		return (term_read_csi());
	return (TERM_EOK);
}

t_term_err	term_read_mod_shift(void)
{
	static const	t_keybind	keys[] = {
		{'D', &select_left},
		{'C', &select_right}
	};
	ssize_t				read_st;
	char				c;
	t_term_action		action;

	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_read_mod_alt(void)
{
	static const t_keybind	keys[] = {
		{'D', select_prev_word},
		{'C', select_next_word}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}

t_term_err	term_del(void)
{
	t_term_err	status;
	ssize_t		read_st;
	char		c;

	status = TERM_EOK;
	if ((read_st = read(STDIN_FILENO, &c, 1)) != 1)
		status = (read_st == 0) ? TERM_EEOF: TERM_EREAD;
	else if (c == '~' && g_term.caps.index != g_term.line->len)
		status = term_line_del(1);
	return (status);
}

t_term_err	term_read_csi(void)
{
	static const t_keybind	mods[] = {
		{'1', &term_read_mod_none},
		{'2', &term_read_mod_shift},
		{'4', &term_read_mod_alt},
	};
	static const t_keybind	keys[] = {
		{'H', &cursor_start_line},
		{'F', &cursor_end_line},
		{'A', &term_prev_line},
		{'B', &term_next_line},
		{'D', &cursor_l},
		{'C', &cursor_r},
		{'3', &term_del}
	};
	ssize_t			read_st;
	char			c;
	t_term_action	action;

	if ((read_st = read(0, &c, 1)) != 1)
		return ((read_st == 0) ? TERM_EEOF: TERM_EREAD);
	if (c == '\0')
		ft_dprintf(2, "[PROMPT][ESC] Alt + [\n");
	if ((action = keybind_get(mods, sizeof(mods) / sizeof(*mods), c)))
		return (action());
	select_clear();
	if ((action = keybind_get(keys, sizeof(keys) / sizeof(*keys), c)))
		return (action());
	return (TERM_EOK);
}
