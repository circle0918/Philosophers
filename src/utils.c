/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:29:02 by yyuan             #+#    #+#             */
/*   Updated: 2021/12/11 14:29:03 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	take_forks(t_philo *philo)
{
	pthread_mutex_lock(philo->lfork);
	printf_mutex(philo, "has taken a fork");
	if (philo->share->arg.total == 1)
		ft_usleep(philo->share->arg.die + 200);
	else
	{
		pthread_mutex_lock(philo->rfork);
		printf_mutex(philo, "has taken a fork");
	}
}

int	is_dead(t_philo **philo, int i)
{
	int	dead;

	pthread_mutex_lock(&philo[0]->share->update_tbc);
	dead = (int)(get_time() - philo[i]->time_begin_count)
		> philo[i]->share->arg.die;
	pthread_mutex_unlock(&philo[0]->share->update_tbc);
	return (dead);
}

int	somebody_dead(t_philo *philo)
{
	int	ret;

	pthread_mutex_lock(&(philo->share->somebody_dead_mutex));
	ret = philo->share->share_dead;
	pthread_mutex_unlock(&(philo->share->somebody_dead_mutex));
	return (ret);
}

//mille second 
unsigned long	get_time(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

//input change to millisecond (usleep's input is mircosecond)
//but more precise than usleep
void	ft_usleep(unsigned long millisecond)
{
	unsigned long	end_time;

	end_time = get_time() + millisecond;
	while (get_time() < end_time)
		usleep(millisecond);
}
