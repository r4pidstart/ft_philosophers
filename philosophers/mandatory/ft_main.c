/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_main.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:07:04 by tjo               #+#    #+#             */
/*   Updated: 2022/12/22 05:32:30 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	is_end(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&philo->table->is_end_lock);
	ret = philo->table->is_end;
	pthread_mutex_unlock(&philo->table->is_end_lock);
	return (ret);
}

void	print_status(t_philo *philo, int status)
{
	pthread_mutex_lock(&philo->table->print_lock);
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
	pthread_mutex_unlock(&philo->table->print_lock);
}

int	dining(t_table *t)
{
	int	i;

	t->start_time = get_time();
	i = 0;
	while (i < t->p_cnt)
	{
		if (pthread_create(&t->philo[i]->thread, 0, &philo, t->philo[i]))
			return (e(THREAD_CREATE_FAILED));
		i++;
	}
	if (pthread_create(&t->monitor, 0, &monitor, t))
		return (e(THREAD_CREATE_FAILED));
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	*table;
	int		i;

	if (!(5 <= argc && argc <= 6))
		return (!printf("Wrong argument(s)\n"));
	if (init_table(argc, argv, &table) || dining(table))
		return (1);
	i = 0;
	while (i < table->p_cnt)
	{
		pthread_join(table->philo[i]->thread, 0);
		i++;
	}
	pthread_join(table->monitor, 0);
}
