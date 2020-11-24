/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   caps.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/24 18:53:27 by chamada           #+#    #+#             */
/*   Updated: 2020/11/24 18:53:52 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "term.h"

static bool	load_caps(t_caps *caps)
{
	char	*a;

	a = NULL;
	caps->mode = (t_modes) {
		tgetstr("im", &a), tgetstr("ei", &a), tgetstr("cl", &a),
		tgetstr("so", &a), tgetstr("se", &a),
	};
	caps->ctrl = (t_ctrls) {
		tgetstr("dc", &a), tgetstr("ec", &a), tgetstr("ce", &a),
		tgetstr("cm", &a), tgetstr("ch", &a), tgetstr("up", &a),
		tgetstr("do", &a), tgetstr("le", &a), tgetstr("nd", &a),
	};
	caps->key = (t_keys) {	
		tgetstr("ku", &a), tgetstr("kd", &a), tgetstr("kl", &a),
		tgetstr("kr", &a),
	};
	return (caps->mode.insert && caps->mode.insert_end && caps->ctrl.del
	&& caps->ctrl.del_line && caps->ctrl.move && caps->ctrl.move_h
	&& caps->ctrl.up && caps->ctrl.down && caps->ctrl.left && caps->ctrl.right
	&& caps->key.up && caps->key.down && caps->key.left && caps->key.right);
}

/*
**	Detect and init the terminal's capabilities
**
**	returns true if successful, or false otherwise.
*/
bool		term_init_caps(t_term *term, t_env **env)
{
	const char	*term_type;
	char		term_buff[MAX_ENTRY + 1];
	int			ent_st;

	if (!env_set(env, "PS1", TERM_PS1, false)
	|| !env_set(env, "PS2", TERM_PS2, false)
	|| !(term_type = env_get(*env, "TERM", 4))
	|| (ent_st = tgetent(term_buff, term_type)) == -1)
		return (false);
	if (ent_st == 0)
		return (true);
	if (tcgetattr(STDIN_FILENO, &term->caps.s_ios) == -1)
		return (false);
	term->caps.s_ios_orig = term->caps.s_ios;
	term->caps.s_ios.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG);
	term->caps.s_ios.c_cflag |= ONLCR;
	if (tcsetattr(STDIN_FILENO, TCSANOW, &term->caps.s_ios) == -1)
		return (false);
	term->has_caps = load_caps(&term->caps);
	return (true);
}
