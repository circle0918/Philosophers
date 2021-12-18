/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:28:54 by yyuan             #+#    #+#             */
/*   Updated: 2021/12/11 14:28:55 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	printf_mutex(t_philo *philo, char *msg)
{
	pthread_mutex_lock(&philo->share->printf_mutex);
	if (philo->share->share_dead == 0)
		printf("%ld\tphilo %d %s\n",
			get_time() - philo->share->start_time, philo->index, msg);
	pthread_mutex_unlock(&philo->share->printf_mutex);
}

void	*printf_die_mutex(t_philo **philo, int i)
{
	int j;

	pthread_mutex_lock(&(philo[0]->share->printf_mutex));
	
	pthread_mutex_lock(&philo[0]->share->somebody_dead_mutex);
	philo[i]->share->share_dead = 1;
	pthread_mutex_unlock(&philo[0]->share->somebody_dead_mutex);
	
	printf("%ld\tphilo %d die\n",
		get_time() - philo[i]->share->start_time, philo[i]->index);
	pthread_mutex_unlock(&(philo[0]->share->printf_mutex));
	j = -1;
	while (++j < philo[0]->share->arg.total)
	{
		if (j != i)
			pthread_join(philo[j]->thread, NULL);
	}
	pthread_mutex_unlock(&philo[i]->finish_mutex);
	pthread_join(philo[i]->thread, NULL);
	return (NULL);
}

void	*printf_must_est_mutex(t_philo **philo)
{	
	int j;
		
	pthread_mutex_lock(&philo[0]->share->printf_mutex);
	
	pthread_mutex_lock(&philo[0]->share->somebody_dead_mutex);
	philo[0]->share->share_dead = 1;
	pthread_mutex_unlock(&philo[0]->share->somebody_dead_mutex);
	
	printf("must eat done\n");
	pthread_mutex_unlock(&philo[0]->share->printf_mutex);
	j = -1;
	while (++j < philo[0]->share->arg.total)
	{
		pthread_join(philo[j]->thread, NULL);
	}
	//pthread_mutex_unlock(&philo[i]->finish_mutex);
	return (NULL);
}

void	exit_free(t_philo **philo, t_share *share,
			t_arg arg, pthread_mutex_t **fork)
{
	int	i;

	pthread_mutex_destroy(&(share->printf_mutex));
	pthread_mutex_destroy(&(share->somebody_dead_mutex));
	free(share->share_philos_finish);
	i = 0;
	while (i < arg.total)
	{
		pthread_mutex_destroy(fork[i]);
		pthread_mutex_destroy(&philo[i]->finish_mutex);
		free(philo[i]);
		free(fork[i]);
		i++;
	}
	free(fork);
	free(philo);
}
