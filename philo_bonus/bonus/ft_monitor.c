/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_monitor.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 04:38:18 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 04:31:30 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static int	philo_check(t_philo *p)
{
	sem_wait(p->eaten_sem);
	if (get_time() - p->last_eaten > p->table->ttd)
	{
		sem_post(p->eaten_sem);
		sem_post(p->table->died_sem);
		print_status(p, DIE);
		return (1);
	}
	if (p->table->must_eat != -1 && p->eat_count >= p->table->must_eat \
		&& p->is_full == 0)
	{
		sem_post(p->table->full_sem);
		p->is_full = 1;
	}
	sem_post(p->eaten_sem);
	return (0);
}

void	*philo_monitor(void *args)
{
	t_table	*table;

	table = (t_table *)args;
	sem_wait(table->died_sem);
	sem_wait(table->full_sem);
	while (!philo_check(table->this))
		usleep(1000);
	return (0);
}

void	kill_philo(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->p_cnt)
	{
		kill(table->pid[i], SIGKILL);
		i++;
	}
}

void	*full_monitor(void *args)
{
	t_table	*table;

	table = (t_table *)args;
	if (table->p_cnt == 1 || table->must_eat == -1)
		return (0);
	usleep(10000);
	while (table->eat_end_cnt < table->p_cnt)
	{
		if (is_end(table))
			return (0);
		sem_wait(table->full_sem);
		if (is_end(table))
			return (0);
		table->eat_end_cnt++;
	}
	sem_wait(table->is_end_sem);
	table->is_end = 1;
	kill_philo(table);
	sem_post(table->died_sem);
	sem_post(table->is_end_sem);
	return (0);
}

void	*died_monitor(void *args)
{
	t_table	*table;

	usleep(10000);
	table = (t_table *)args;
	if (table->p_cnt == 1)
	{
		sem_wait(table->is_end_sem);
		table->is_end = 1;
		sem_post(table->is_end_sem);
		return (0);
	}
	if (is_end(table))
		return (0);
	sem_wait(table->died_sem);
	if (is_end(table))
		return (0);
	sem_wait(table->is_end_sem);
	table->is_end = 1;
	kill_philo(table);
	sem_post(table->full_sem);
	sem_post(table->is_end_sem);
	return (0);
}