/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_utils.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjo <tjo@student.42seoul.kr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/23 04:36:27 by tjo               #+#    #+#             */
/*   Updated: 2022/12/23 09:30:35 by tjo              ###   ########.fr       */
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

int	ft_atoi_check(const char *str, int *chk)
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
		|| ret < INT32_MIN || ret > INT32_MAX || minus_cnt)
		(*chk)++;
	return (ret);
}

time_t	get_time(void)
{
	struct timeval		tv;

	gettimeofday(&tv, 0);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
