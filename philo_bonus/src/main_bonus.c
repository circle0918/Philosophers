/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 12:08:24 by yyuan             #+#    #+#             */
/*   Updated: 2021/11/30 22:15:57 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "philosophers_bonus.h"

void	exit_free(t_philo **philo, t_share *share, t_arg arg, sem_t *forks)
{
	int	i;

	i = 0;
	while (i < arg.total)
	{
		kill(philo[i]->pid, SIGTERM);
		free(philo[i]);
		i++;
	}
	free(philo);
	sem_close(forks);
	sem_close(share->must_eat_done);
	sem_close(share->print);
	sem_close(share->update_tbc);
	sem_close(share->update_dead);
	sem_unlink("forks");
	sem_unlink("must_eat_done");
	sem_unlink("print");
	sem_unlink("update_tbc");
	sem_unlink("update_dead");
}

void	*counter(t_philo *philo)
{
	int	is_dead;

	while (1)
	{
		sem_wait(philo->share->update_tbc);
		is_dead = (int)(get_time() - philo->time_begin_count)
			> philo->share->arg.die;
		sem_post(philo->share->update_tbc);
		if (is_dead)
			exit(printf_die_mutex(philo, philo->index));
		ft_usleep(1);
	}
	return (NULL);
}

void	*share_counter(t_philo **philo)
{
	int	i;

	i = 0;
	while (i < philo[0]->share->arg.total)
	{
		sem_wait(philo[0]->share->must_eat_done);
		i++;
	}
	exit(printf_must_est_mutex(philo));
	return (NULL);
}

void	*activites(t_philo *philo)
{
	while (1)
	{
		sem_wait(philo->forks);
		printf_mutex(philo, "has taken a fork");
		printf_mutex(philo, "has taken a fork");
		sem_wait(philo->share->update_tbc);
		philo->time_begin_count = get_time();
		sem_post(philo->share->update_tbc);
		printf_mutex(philo, "is eating");
		ft_usleep(philo->share->arg.eat);
		sem_post(philo->forks);
		philo->eat_count++;
		if (philo->eat_count == philo->share->arg.must_eat)
			sem_post(philo->share->must_eat_done);
		printf_mutex(philo, "is sleeping");
		ft_usleep(philo->share->arg.sleep);
		printf_mutex(philo, "is thinking");
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_arg		arg;
	t_philo		**philo;
	sem_t		*forks;
	t_share		share;

	if (parser(ac, av, &arg))
		return (0);
	philo = malloc(sizeof (t_philo) * arg.total);
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, S_IRWXU, arg.total / 2);
	if (!philo || !forks)
		return (-1);
	init(philo, &share, arg, forks);
	waitpid(-1, NULL, 0);
	exit_free(philo, &share, arg, forks);
	return (0);
}
