/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:27:40 by yyuan             #+#    #+#             */
/*   Updated: 2021/12/11 14:27:43 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	*share_counter(t_philo **philo)
{
	int	i;
	int	must_eat_done;
	int	is_finish;

	while (1)
	{
		i = -1;
		must_eat_done = 1;
		while (++i < philo[0]->share->arg.total)
		{
			pthread_mutex_lock(&philo[i]->finish_mutex);
			if (is_dead(philo, i))
				return (printf_die_mutex(philo, i));
			pthread_mutex_lock(&(philo[0]->share->update_ec));
			is_finish = philo[i]->eat_count >= philo[i]->share->arg.must_eat;
			pthread_mutex_unlock(&(philo[0]->share->update_ec));
			must_eat_done = (must_eat_done && is_finish);
			pthread_mutex_unlock(&philo[i]->finish_mutex);
		}
		if (must_eat_done)
			return (printf_must_est_mutex(philo));
		ft_usleep(1);
	}
	return (NULL);
}

void	*activites(t_philo *philo)
{
	while (!(somebody_dead(philo))
		&& philo->eat_count < philo->share->arg.must_eat)
	{
		take_forks(philo);
		pthread_mutex_lock(&philo->share->update_tbc);
		philo->time_begin_count = get_time();
		pthread_mutex_unlock(&philo->share->update_tbc);
		printf_mutex(philo, "is eating");
		ft_usleep(philo->share->arg.eat);
		if (philo->share->arg.total != 1)
			pthread_mutex_unlock(philo->rfork);
		pthread_mutex_unlock(philo->lfork);
		pthread_mutex_lock(&(philo->share->update_ec));
		philo->eat_count++;
		pthread_mutex_unlock(&(philo->share->update_ec));
		printf_mutex(philo, "is sleeping");
		ft_usleep(philo->share->arg.sleep);
		printf_mutex(philo, "is thinking");
		usleep(150);
	}
	return (NULL);
}

void	init_philo(t_philo **philo,
	t_share *share, pthread_mutex_t **fork, int i)
{
	while (i < share->arg.total)
	{
		philo[i] = malloc(sizeof (t_philo));
		philo[i]->share = share;
		philo[i]->lfork = fork[i];
		philo[i]->time_begin_count = share->start_time;
		philo[i]->eat_count = 0;
		if (i == share->arg.total - 1)
		{
			philo[i]->rfork = fork[i];
			philo[i]->lfork = fork[0];
		}
		else
			philo[i]->rfork = fork[i + 1];
		philo[i]->index = i + 1;
		pthread_mutex_init(&philo[i]->finish_mutex, NULL);
		pthread_create(&(philo[i]->thread), NULL, (void *)&activites, philo[i]);
		i = i + 2;
		usleep(50);
	}
}

void	init(t_philo **philo, t_share *share, t_arg arg, pthread_mutex_t **fork)
{
	int	i;

	share->arg = arg;
	share->share_dead = 0;
	share->start_time = get_time();
	pthread_mutex_init(&(share->somebody_dead_mutex), NULL);
	pthread_mutex_init(&share->printf_mutex, NULL);
	pthread_mutex_init(&share->update_tbc, NULL);
	pthread_mutex_init(&share->update_ec, NULL);
	share->share_philos_finish = malloc(sizeof(char) * arg.total);
	memset(share->share_philos_finish, 0, arg.total);
	i = 0;
	while (i < arg.total)
	{
		fork[i] = malloc(sizeof (pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
		i++;
	}
	init_philo(philo, share, fork, 0);
	ft_usleep(1);
	init_philo(philo, share, fork, 1);
	pthread_create(&(share->share_thread), NULL, (void *)&share_counter, philo);
	pthread_join(share->share_thread, 0);
}

int	main(int ac, char **av)
{
	t_arg			arg;
	t_philo			**philo;
	pthread_mutex_t	**fork;
	t_share			share;

	if (parser(ac, av, &arg))
		return (0);
	philo = malloc(sizeof (t_philo) * arg.total);
	fork = malloc(sizeof (pthread_mutex_t) * arg.total);
	if (!philo || !fork)
		return (-1);
	init(philo, &share, arg, fork);
	exit_free(philo, &share, arg, fork);
	return (0);
}
