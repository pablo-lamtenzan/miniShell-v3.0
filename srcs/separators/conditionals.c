/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditionals.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/05 00:01:31 by pablo             #+#    #+#             */
/*   Updated: 2020/11/17 12:21:59 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <separators.h>
#include <execution.h>

int	handle_conditionals(t_term** term, int parser_st, int* flags, int parentheses_nb)
{
	if (parser_st & CLOSE_PAR)
	{
		ft_dprintf(2, "Contionals close par!\n");
		if ((*flags & SKIP && *flags & SKIPED_AND) || (!(*flags & SKIP) && !(*flags & NOT_SKIPED_OR)))
			(*term)->st = 127;
		*flags &= ~SKIP;
	}
	if (parser_st & SEMICOLON)
		*flags &= ~SKIP;
	if (*flags & SKIP)
		return (false);
	ft_dprintf(2, "CONDITIONALS: st = [%d]\n", (*term)->st);
	if (parser_st & AND && (*term)->st) 
		return (!(*flags = SKIP | (parentheses_nb ? SKIPED_AND : 0)));
	if (parser_st & OR && !(*term)->st) 
	{
		*flags &= ~NOT_SKIPED_OR;
		return (!(*flags |= SKIP));
	}
	else if (parentheses_nb)
		*flags |= NOT_SKIPED_OR;
	if (!(*flags & SKIP)) 
		*flags = *flags & NOT_SKIPED_OR ? NOT_SKIPED_OR : NONE;
	return (true);
}