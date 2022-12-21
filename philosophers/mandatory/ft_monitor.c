/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 04:38:18 by tjo               #+#    #+#             */
/*   Updated: 2022/12/22 05:41:59 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static int	set_end(t_table *t)
{
	pthread_mutex_lock(&t->is_end_lock);
	t->is_end = 1;
	pthread_mutex_unlock(&t->is_end_lock);
	return (1);
}

static int	end_check(t_table *t)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (i < t->p_cnt)
	{
		pthread_mutex_lock(&t->philo[i]->eaten_lock);
		if (get_time() - t->philo[i]->last_eaten > t->ttd)
		{
			print_status(t->philo[i], DIE);
			pthread_mutex_unlock(&t->philo[i]->eaten_lock);
			return (set_end(t));
		}
		if (t->must_eat != -1 && t->philo[i]->eat_count < t->must_eat)
			flag = 1;
		pthread_mutex_unlock(&t->philo[i]->eaten_lock);
		i++;
	}
	if (t->must_eat != -1 && !flag)
		return (set_end(t));
	return (0);
}

void	*monitor(void *args)
{
	if (((t_table *)args)->p_cnt == 1)
		return (0);
	while (1)
	{
		if (end_check((t_table *)args))
			return (0);
		usleep(100);
	}
	return (0);
}