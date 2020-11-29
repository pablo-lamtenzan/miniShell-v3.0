/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   jobspec_parser_utils.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:51:20 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 03:53:07 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <libft.h>

size_t			get_search_mode(const char *av)
{
	ft_dprintf(2, "AV : %s\n", av);
	if (av[0] && av[0] != '?')
		return (1);
	if (av[0] == '?' && av[1])
		return (2);
	else
		return (0);
}

bool			is_history_process(const char *string)
{
	return (string[0] == '%' && (!string[1] || (!string[2] \
		&& (string[1] == '+' || string[1] == '-' || string[1] == '%'))));
}

size_t			get_history_index(const char *key)
{
	if (key[0] == '+' || key[0] == '%' || key[0] == '\0')
		return (1);
	if (key[0] == '-')
		return (2);
	else
		return (0);
}
