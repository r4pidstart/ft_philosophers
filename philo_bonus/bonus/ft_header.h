/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_header.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:08:35 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 05:04:27 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_HEADER_H
# define FT_HEADER_H

# include<stdio.h>
# include<semaphore.h>
# include<unistd.h>
# include<stdlib.h>
# include<sys/time.h>
# include<pthread.h>
# include<signal.h>

typedef pthread_mutex_t	t_mutex;

typedef struct s_table	t_table;

typedef struct s_philo
{
	pthread_t	philo_monitor;
	int			id;
	int			eat_count;
	int			is_full;
	time_t		last_eaten;
	sem_t		*eaten_sem;
	char		*eaten_sem_name;
	t_table		*table;
}t_philo;

typedef struct s_table
{
	pthread_t	die_monitor;
	pthread_t	full_monitor;
	t_philo		**philo;
	t_philo		*this;
	int			p_cnt;
	time_t		start_time;
	time_t		ttd;
	time_t		tte;
	time_t		tts;
	int			must_eat;
	int			eat_end_cnt;
	sem_t		*full_sem;
	sem_t		*fork_sem;
	sem_t		*print_sem;
	sem_t		*died_sem;
	sem_t		*is_end_sem;
	int			is_end;
	pid_t		*pid;
}t_table;

// ft_main.c
time_t	get_time(void);
int		is_end(t_table *table);
void	print_status(t_philo *philo, int status);
// ft_init.c
int		init_table(int argc, char **argv, t_table **table);
// ft_philo.c
void	*philo(void *args);
// ft_monitor.c
void	*philo_monitor(void *args);
void	*died_monitor(void *args);
void	*full_monitor(void *args);
void	kill_philo(t_table *table);
// ft_error.c
int		e(const char *msg);
// ft_utils1.c
int		ft_atoi_check(const char *str, int *chk);
char	*ft_strjoin(char const *s1, char const *s2);
// ft_utils2.c
char	*ft_itoa(int n);

# define MALLOC_FAILED "MALLOC FAILED\n"
# define WRONG_ARGUMENT "WRONG_ARGUMENT\n"
# define MUTEX_INIT_FAILED "MUTEX INIT FAILED\n"
# define THREAD_CREATE_FAILED "THREAD CREATE FAILED\n"
# define SEM_OPEN_FAILED "SEM OPEN FAILED\n"
# define FORK_FAILED "FORK FAILED\n"

# define FORK_SEM "FORK_SEM"
# define PRINT_SEM "PRINT_SEM"
# define EATEN_SEM "EATEN_SEM"
# define FULL_SEM "FULL_SEM"
# define DIED_SEM "DIED_SEM"
# define IS_END_SEM "IS_END_SEM"

enum	e_philo_status
{
	THINK = 0,
	EAT = 1,
	SLEEP = 2,
	DIE = 3,
	FORK = 4
};

#endif