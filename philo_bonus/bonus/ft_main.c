/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:07:04 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 05:16:25 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	is_end(t_table *table)
{
	int	ret;

	sem_wait(table->is_end_sem);
	ret = table->is_end;
	sem_post(table->is_end_sem);
	return (ret);
}

void	print_status(t_philo *philo, int status, int end)
{
	sem_wait(philo->table->print_sem);
	printf("%ld %d ", get_time() - philo->table->start_time, philo->id + 1);
	if (status == THINK)
		printf("is thinking\n");
	else if (status == EAT)
		printf("is eating\n");
	else if (status == SLEEP)
		printf("is sleeping\n");
	else if (status == DIE)
		printf("died\n");
	else if (status == FORK)
		printf("has taken a fork\n");
	if (!end)
		sem_post(philo->table->print_sem);
}

int	dining(t_table *t)
{
	int		i;
	pid_t	pid;

	t->start_time = get_time();
	i = 0;
	while (i < t->p_cnt)
	{
		pid = fork();
		if (pid == -1)
			return (e(FORK_FAILED));
		else if (pid == 0)
		{
			t->this = t->philo[i];
			philo(t);
		}
		else
			t->pid[i] = pid;
		i++;
	}
	if (pthread_create(&t->die_monitor, 0, &died_monitor, t))
		return (e(THREAD_CREATE_FAILED));
	if (pthread_create(&t->full_monitor, 0, &full_monitor, t))
		return (e(THREAD_CREATE_FAILED));
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	*table;

	if (!(5 <= argc && argc <= 6))
		return (!printf("Wrong argument(s)\n"));
	if (init_table(argc, argv, &table) || dining(table))
		return (1);
	while (!is_end(table))
		usleep(10000);
}
