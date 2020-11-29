/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   conditions.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pablo <pablo@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/27 01:00:32 by pablo             #+#    #+#             */
/*   Updated: 2020/11/29 04:10:57 by pablo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <job_control.h>
#include <sys/wait.h>

bool			is_active(t_process *target)
{
	return (target->flags & (BACKGROUND | STOPPED | SIGNALED));
}

bool			is_exited(t_process *target)
{
	return (target->flags & EXITED);
}

bool			is_removable(t_process *target)
{
	return (!(target->flags & NO_DELETE));
}

bool			is_coredump(t_process *target)
{
	return (WCOREDUMP(target->wstatus));
}

bool 			is_signaled(t_process *target)
{
	return (target->flags & SIGNALED);
}