/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_separators.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/23 10:27:06 by pablo             #+#    #+#             */
/*   Updated: 2020/11/23 12:10:00 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <execution.h>

size_t		get_separators_nb(char* input, char** separators)
{
	size_t	size;
	size_t	aux;
	int		i;
	int		y;

	i = -1;
	size = 0;
	while (input[++i])
	{
		y = -1;
		while (separators[++y])
		{
			if (!ft_strncmp(separators[y], &input[i], aux = ft_strlen(separators[y])))
			{
				size++;
				i += (int)aux - 1; // or good or 1 more further
				break ;
			}
		}
	}
	return (size);
}

size_t		get_elem_size(char* input, int *i, char** separators)
{
	int		y;
	size_t	size;

	size = 1;
	while (input[++(*i)])
	{
		y = -1;
		while (separators[++y])
		{
			if (!ft_strncmp(separators[y], &input[*i], ft_strlen(separators[y])))
				return (size);
		}
		size++;
	}
	return (size);
}

int			copy_inter_seps(char*** res, char* input, char** separators)
{
	int		y;
	int		i;
	int		z;
	size_t	aux;

	i = 0;
	z = 0;
	while (input[i])
	{
		y = -1;
		while (separators[++y])
		{
			if (!ft_strncmp(&input[i], separators[y], aux = ft_strlen(separators[y]))) // skip separators
			{
				i += (int)aux;
				y = -1;
			}
		}
		int remember = i;
		if ((aux = get_elem_size(input, &i, separators)))
		{
			if (!((*res)[z++] = ft_strndup(&input[remember], aux)))
				return (false);
		}
	}
	return (true);
}

char**		split_separators(char* input, char** separators)
{
	char**	res;
	size_t	size;
	
	if (PRINT_DEBUG)
	if (!(res = ft_calloc((size = get_separators_nb(input, separators)) + 1, sizeof(char*))))
		return (NULL);
	if (!(copy_inter_seps(&res, input, separators)))
		return (false); // TO DO: free in case of error
	res[size] = NULL;
	return (res);
}