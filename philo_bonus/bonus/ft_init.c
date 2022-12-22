/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:41:36 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 05:50:40 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

int	init_philo_eaten_sem_name(t_philo *philo)
{
	char	*tmp;

	tmp = ft_itoa(philo->id);
	if (!tmp)
		return (e(MALLOC_FAILED));
	philo->eaten_sem_name = ft_strjoin(EATEN_SEM, tmp);
	if (!philo->eaten_sem_name)
		return (e(MALLOC_FAILED));
	free(tmp);
	return (0);
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
		if (init_philo_eaten_sem_name((*table)->philo[i]))
			return (1);
		(*table)->philo[i]->id = i;
		(*table)->philo[i]->eat_count = 0;
		(*table)->philo[i]->last_eaten = get_time();
		(*table)->philo[i]->table = *table;
		(*table)->philo[i]->is_full = 0;
		i++;
	}
	return (0);
}

static int	init_table_lock(t_table **t)
{
	sem_unlink(IS_END_SEM);
	sem_unlink(FORK_SEM);
	sem_unlink(PRINT_SEM);
	sem_unlink(FULL_SEM);
	sem_unlink(DIED_SEM);
	(*t)->is_end_sem = sem_open(IS_END_SEM, O_CREAT, 0644, 1);
	(*t)->fork_sem = sem_open(FORK_SEM, O_CREAT, 0644, (*t)->p_cnt);
	(*t)->print_sem = sem_open(PRINT_SEM, O_CREAT, 0644, 1);
	(*t)->full_sem = sem_open(FULL_SEM, O_CREAT, 0644, (*t)->p_cnt);
	(*t)->died_sem = sem_open(DIED_SEM, O_CREAT, 0644, (*t)->p_cnt);
	if ((*t)->is_end_sem == SEM_FAILED || (*t)->fork_sem == SEM_FAILED || \
		(*t)->print_sem == SEM_FAILED || (*t)->full_sem == SEM_FAILED || \
		(*t)->died_sem == SEM_FAILED)
		return (e(SEM_OPEN_FAILED));
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
	(*table)->pid = (pid_t *)malloc(sizeof(pid_t) * (*table)->p_cnt);
	(*table)->is_end = 0;
	if (chk || (*table)->p_cnt == 0)
		return (e(WRONG_ARGUMENT));
	if (init_philo(table))
		return (1);
	if (init_table_lock(table) || (*table)->must_eat == 0)
		return (1);
	return (0);
}
