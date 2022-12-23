/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_header.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:08:35 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 09:30:44 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEADER_H
# define FT_HEADER_H

# include<stdio.h>
# include<pthread.h>
# include<unistd.h>
# include<stdlib.h>
# include<sys/time.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_table	t_table;

typedef struct s_philo
{
	pthread_t	thread;
	int			id;
	int			eat_count;
	int			fork[2];
	time_t		last_eaten;
	t_mutex		eaten_lock;
	t_table		*table;
}t_philo;

typedef struct s_table
{
	pthread_t	monitor;
	t_philo		**philo;
	int			p_cnt;
	time_t		start_time;
	time_t		ttd;
	time_t		tte;
	time_t		tts;
	int			must_eat;
	t_mutex		*fork_lock;
	t_mutex		print_lock;
	t_mutex		is_end_lock;
	int			is_end;
}t_table;

// ft_main.c
int		is_end(t_philo *philo);
void	print_status(t_philo *philo, int status);
// ft_init.c
int		init_table(int argc, char **argv, t_table **table);
// ft_philo.c
void	*philo(void *args);
// ft_monitor.c
void	*monitor(void *args);
// ft_error.c
int		e(const char *msg);
// ft_utils.c
time_t	get_time(void);
int		ft_atoi_check(const char *str, int *chk);

# define MALLOC_FAILED "MALLOC FAILED\n"
# define WRONG_ARGUMENT "WROND ARGUMENT\n"
# define MUTEX_INIT_FAILED "MUTEX INIT FAILED\n"
# define THREAD_CREATE_FAILED "THREAD CREATE FAILED\n"

enum	e_philo_status
{
	THINK = 0,
	EAT = 1,
	SLEEP = 2,
	DIE = 3,
	FORK = 4
};

#endif