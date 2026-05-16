/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock_unlock.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:04:21 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/10 21:04:22 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	lock_two_dongles(t_coder *coder)
{
	if (coder->lowest == coder->highest)
	{
		pthread_mutex_lock(&coder->lowest->mutex);
		return ;
	}
	pthread_mutex_lock(&coder->lowest->mutex);
	pthread_mutex_lock(&coder->highest->mutex);
}

void	unlock_two_dongles(t_coder *coder)
{
	if (coder->lowest == coder->highest)
	{
		pthread_mutex_unlock(&coder->lowest->mutex);
		return ;
	}
	pthread_mutex_unlock(&coder->highest->mutex);
	pthread_mutex_unlock(&coder->lowest->mutex);
}
