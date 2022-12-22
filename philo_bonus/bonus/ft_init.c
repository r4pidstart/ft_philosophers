/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/22 02:41:36 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 04:11:37 by tjo              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"ft_header.h"

static size_t	ft_strlen(const char *s)
{
	size_t	n;

	n = 0;
	while (s[n])
		n++;
	return (n);
}

static int	ft_atoi_check(const char *str, int *chk)
{
	char		*cur;
	int			minus_cnt;
	long long	ret;

	cur = (char *)str;
	while ((9 <= *cur && *cur <= 13) || *cur == 20 || *cur == 32)
		cur++;
	ret = 0;
	minus_cnt = 0;
	if ('+' == *cur || '-' == *cur)
	{
		if (*cur == '-')
			minus_cnt++;
		cur++;
	}
	if (*cur == '\0')
		(*chk)++;
	while ('0' <= *cur && *cur <= '9')
		ret = ret * 10 + (*(cur++) - '0');
	if (minus_cnt)
		ret *= -1;
	if (*cur != '\0' || ft_strlen(str) > 11 \
		|| ret < INT32_MIN || ret > INT32_MAX)
		(*chk)++;
	return (ret);
}

int	get_length(long long n)
{
	int	ret;

	ret = 0;
	if (n == 0)
		ret++;
	if (n < 0)
	{
		ret++;
		n *= -1;
	}
	while (n)
	{
		ret++;
		n /= 10;
	}
	return (ret);
}

char	*ft_itoa(int n)
{
	long long	tmp;
	char		*ret;
	int			cur;

	ret = (char *)malloc(sizeof(char) * get_length(n) + 1);
	if (!ret)
		return (0);
	tmp = n;
	cur = 0;
	if (n < 0)
	{
		ret[cur++] = '-';
		tmp *= -1;
	}
	if (n == 0)
		ret[cur++] = '0';
	cur = get_length(n);
	ret[cur] = '\0';
	while (tmp)
	{
		ret[--cur] = tmp % 10 + '0';
		tmp /= 10;
	}
	return (ret);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t	cur;

	if (dst == src)
		return (dst);
	if (dst < src)
	{
		cur = 0;
		while (0 <= cur && cur < len)
		{
			*((char *)dst + cur) = *((char *)src + cur);
			cur++;
		}
	}
	else
	{
		cur = len - 1;
		while (0 <= cur && cur < len)
		{
			*((char *)dst + cur) = *((char *)src + cur);
			cur--;
		}
	}
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*ret;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (0);
	s1_len = ft_strlen(s1);
	s2_len = ft_strlen(s2);
	ret = (char *)malloc(sizeof(char) * s1_len + s2_len + 1);
	if (!ret)
		return (0);
	ft_memmove(ret, s1, s1_len);
	ft_memmove(ret + s1_len, s2, s2_len);
	ret[s1_len + s2_len] = 0;
	return (ret);
}

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
	if (chk || (*table)->p_cnt <= 0 || (*table)->ttd < 0 \
		|| (*table)->tte < 0 || (*table)->tts < 0)
		return (e(WRONG_ARGUMENT));
	if (init_philo(table))
		return (1);
	if (init_table_lock(table))
		return (1);
	if ((*table)->must_eat == 0)
		return (1);
	return (0);
}