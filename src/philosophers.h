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
#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_arg
{
	int	total;
	int	die;
	int	eat;
	int	sleep;
	int	must_eat;
}	t_arg;

typedef struct s_share
{
	t_arg				arg;
	unsigned long		start_time;
	pthread_mutex_t		printf_mutex;
	pthread_mutex_t		update_tbc;
	pthread_mutex_t		update_ec;
	pthread_mutex_t		somebody_dead_mutex;
	pthread_t			share_thread;
	int					share_dead;
	char					*share_philos_finish;
}	t_share;

typedef struct s_philo
{
	int				index;
	int				finish_must_eat;
	int				eat_count;
	unsigned long	time_begin_count;
	pthread_t		thread;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	pthread_mutex_t	finish_mutex;
	t_share			*share;
}	t_philo;

int				ft_atoi(const char *str);
int				isnum(int ac, char **av);
int				parser(int ac, char **av, t_arg *arg);
unsigned long	get_time(void);
void			ft_usleep(unsigned long millisecond);
void			printf_mutex(t_philo *philo, char *msg);
void			*printf_die_mutex(t_philo **philo, int i);
void			*printf_must_est_mutex(t_philo **philo);
void			*share_counter(t_philo **philo);
void			*activites(t_philo *philo);
void			init_philo(t_philo **philo, t_share *share,
					pthread_mutex_t **fork, int i);
void			init(t_philo **philo, t_share *share,
					t_arg arg, pthread_mutex_t **fork);
void			exit_free(t_philo **philo, t_share *share,
					t_arg arg, pthread_mutex_t **fork);

#endif
