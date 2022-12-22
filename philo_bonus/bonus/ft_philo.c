/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_philo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 04:23:22 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 05:07:40 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static void	philo_sleep(t_philo *philo, time_t wakeup_time)
{
	while (get_time() < wakeup_time)
	{
		if (is_end(philo->table))
			break ;
		usleep(100);
	}
}

static void	philo_think(t_philo *philo, int flag)
{
	time_t	sleep_time;

	sleep_time = (philo->table->ttd - \
				(get_time() - philo->last_eaten) \
				- philo->table->tte) / 2;
	if (sleep_time < 0)
		sleep_time = 0;
	if (sleep_time > 100)
		sleep_time = 100;
	if (!flag)
		print_status(philo, THINK);
	if (sleep_time)
		philo_sleep(philo, get_time() + sleep_time);
}

static void	philo_eat_sleep(t_philo *philo)
{
	sem_wait(philo->table->fork_sem);
	print_status(philo, FORK);
	sem_wait(philo->table->fork_sem);
	print_status(philo, FORK);
	print_status(philo, EAT);
	sem_wait(philo->eaten_sem);
	philo->last_eaten = get_time();
	philo->eat_count++;
	sem_post(philo->eaten_sem);
	philo_sleep(philo, get_time() + philo->table->tte);
	sem_post(philo->table->fork_sem);
	sem_post(philo->table->fork_sem);
	print_status(philo, SLEEP);
	philo_sleep(philo, get_time() + philo->table->tts);
}

int	init_philo_process(t_philo *p)
{
	sem_unlink(p->eaten_sem_name);
	p->eaten_sem = sem_open(p->eaten_sem_name, O_CREAT, 0644, 1);
	if (p->eaten_sem == SEM_FAILED)
		return (1);
	if (pthread_create(&p->philo_monitor, 0, &philo_monitor, p->table))
		return (1);
	return (0);
}

void	*philo(void *args)
{
	t_philo	*p;

	p = ((t_table *)args)->this;
	if (p->table->p_cnt == 1)
	{
		print_status(p, FORK);
		philo_sleep(p, get_time() + p->table->ttd);
		print_status(p, DIE);
		return (0);
	}
	if (init_philo_process(p))
		return (0);
	sem_wait(p->eaten_sem);
	p->last_eaten = get_time();
	sem_post(p->eaten_sem);
	if (p->id & 1)
		philo_sleep(p, p->last_eaten + 1);
	while (1)
	{
		philo_eat_sleep(p);
		philo_think(p, 0);
	}	
}
