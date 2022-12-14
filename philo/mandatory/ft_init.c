/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:41:36 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 05:50:59 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static void	fork_assign(int i, t_table *table)
{
	if (i & 1)
	{
		table->philo[i]->fork[0] = i;
		table->philo[i]->fork[1] = (i + 1) % table->p_cnt;
	}
	else
	{
		table->philo[i]->fork[1] = i;
		table->philo[i]->fork[0] = (i + 1) % table->p_cnt;
	}
}

static int	init_philo(t_table **table)
{
	int	i;

	(*table)->philo = (t_philo **)malloc(sizeof(t_philo *) * (*table)->p_cnt);
	if (!(*table)->philo)
		return (e(MALLOC_FAILED));
	i = 0;
	while (i < (*table)->p_cnt)
	{
		(*table)->philo[i] = (t_philo *)malloc(sizeof(t_philo));
		if (!(*table)->philo[i])
			return (e(MALLOC_FAILED));
		if (pthread_mutex_init(&(*table)->philo[i]->eaten_lock, 0))
			return (e(MUTEX_INIT_FAILED));
		(*table)->philo[i]->id = i;
		(*table)->philo[i]->eat_count = 0;
		(*table)->philo[i]->last_eaten = get_time();
		fork_assign(i, *table);
		(*table)->philo[i]->table = *table;
		i++;
	}
	return (0);
}

static int	init_table_lock(t_table **table)
{
	int	i;

	(*table)->fork_lock = (t_mutex *)malloc(sizeof(t_mutex) * (*table)->p_cnt);
	if (!((*table)->fork_lock))
		return (e(MALLOC_FAILED));
	i = 0;
	while (i < (*table)->p_cnt)
	{
		if (pthread_mutex_init(&(*table)->fork_lock[i], 0))
			return (e(MUTEX_INIT_FAILED));
		i++;
	}
	if (pthread_mutex_init(&(*table)->is_end_lock, 0))
		return (e(MUTEX_INIT_FAILED));
	if (pthread_mutex_init(&(*table)->print_lock, 0))
		return (e(MUTEX_INIT_FAILED));
	return (0);
}

int	init_table(int argc, char **argv, t_table **table)
{
	int	chk;

	*table = (t_table *)malloc(sizeof(t_table));
	if (!(*table))
		return (e(MALLOC_FAILED));
	chk = 0;
	(*table)->p_cnt = ft_atoi_check(argv[1], &chk);
	(*table)->ttd = ft_atoi_check(argv[2], &chk);
	(*table)->tte = ft_atoi_check(argv[3], &chk);
	(*table)->tts = ft_atoi_check(argv[4], &chk);
	if (argc == 6)
		(*table)->must_eat = ft_atoi_check(argv[5], &chk);
	else
		(*table)->must_eat = -1;
	(*table)->is_end = 0;
	if (chk || (*table)->p_cnt == 0)
		return (e(WRONG_ARGUMENT));
	if (init_philo(table))
		return (1);
	if (init_table_lock(table))
		return (1);
	if ((*table)->must_eat == 0)
		return (1);
	return (0);
}
