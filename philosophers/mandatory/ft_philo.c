/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 04:23:22 by tjo               #+#    #+#             */
/*   Updated: 2022/12/22 04:58:54 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static void	philo_sleep(t_philo *philo, time_t wakeup_time)
{
	while (get_time() < wakeup_time)
	{
		if (is_end(philo))
			break ;
		usleep(100);
	}
}

static void	philo_think(t_philo *philo, int flag)
{
	time_t	sleep_time;

	sleep_time = (philo->table->ttd - \
				(get_time() - philo->last_eaten) \
				- philo->table->tte);
	if (sleep_time < 0)
		sleep_time = 0;
	if (!flag)
		print_status(philo, THINK);
	philo_sleep(philo, get_time() + sleep_time);
}

static void	philo_eat_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_lock[philo->fork[0]]);
	print_status(philo, FORK);
	pthread_mutex_lock(&philo->table->fork_lock[philo->fork[1]]);
	print_status(philo, FORK);
	print_status(philo, EAT);
	pthread_mutex_lock(&philo->eaten_lock);
	philo->last_eaten = get_time();
	philo->eat_count++;
	pthread_mutex_unlock(&philo->eaten_lock);
	philo_sleep(philo, get_time() + philo->table->tte);
	print_status(philo, SLEEP);
	pthread_mutex_unlock(&philo->table->fork_lock[philo->fork[0]]);
	pthread_mutex_unlock(&philo->table->fork_lock[philo->fork[1]]);
	philo_sleep(philo, get_time() + philo->table->tts);
}

static void	*only_one_philo(t_philo *philo)
{
	pthread_mutex_lock(&philo->table->fork_lock[0]);
	print_status(philo, FORK);
	philo_sleep(philo, get_time() + philo->table->ttd);
	print_status(philo, DIE);
	pthread_mutex_unlock(&philo->table->fork_lock[0]);
	return (0);
}

void	*philo(void *args)
{
	t_philo	*p;

	p = (t_philo *)args;
	p->last_eaten = get_time();
	if (p->table->ttd == 0)
		return (0);
	else if (p->id & 1)
		philo_think(p, 1);
	if (p->table->p_cnt == 1)
		return (only_one_philo(p));
	while (!is_end(p))
	{
		philo_eat_sleep(p);
		philo_think(p, 0);
	}
	return (0);
}
