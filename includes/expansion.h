/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chamada <chamada@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/14 01:31:51 by chamada           #+#    #+#             */
/*   Updated: 2020/12/06 00:03:30 by chamada          ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPANSION_H
# define EXPANSION_H

# include <lexer/token.h>
# include <env.h>
# include <term/line.h>

# define EXP_TILDE	"HOME"
# define EXP_TILDEP	"PWD"
# define EXP_TILDEM	"OLDPWD"

typedef enum	e_exp_st
{
	EXP_EALLOC = -2,
	EXP_ENOMATCH = -1,
	EXP_EOK = 0
}				t_exp_st;

char			**tokens_expand(t_tok **tokens, t_env **env, int *ac);

bool			param_expand(t_tok *param, t_env *env);
t_line			*string_expand(const char *input, t_env *env);

t_line			*word_split(t_tok **params);

#endif
