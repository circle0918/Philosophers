/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:28:44 by yyuan             #+#    #+#             */
/*   Updated: 2021/12/11 14:28:46 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers_bonus.h"

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

void	printf_mutex(t_philo *philo, char *msg)
{
	int	is_dead;

	sem_wait(philo->share->print);
	sem_wait(philo->share->update_dead);
	is_dead = philo->share->share_dead;
	sem_post(philo->share->update_dead);
	if (is_dead == 0)
		printf("%ld\tphilo %d %s\n",
			get_time() - philo->share->start_time, philo->index, msg);
	sem_post(philo->share->print);
}

int	printf_die_mutex(t_philo *philo, int i)
{
	sem_wait(philo->share->update_dead);
	philo->share->share_dead = 1;
	sem_post(philo->share->update_dead);
	sem_wait(philo->share->print);
	printf("%ld\tphilo %d die\n", get_time() - philo->share->start_time, i);
	return (1);
}

int	printf_must_est_mutex(t_philo **philo)
{			
	sem_wait(philo[0]->share->update_dead);
	philo[0]->share->share_dead = 1;
	sem_post(philo[0]->share->update_dead);
	sem_wait(philo[0]->share->print);
	printf("must eat done\n");
	return (1);
}
