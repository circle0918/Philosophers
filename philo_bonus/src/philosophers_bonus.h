/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers_bonus.h                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/12/11 14:28:33 by yyuan             #+#    #+#             */
/*   Updated: 2021/12/11 17:06:02 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_BONUS_H
# define PHILOSOPHERS_BONUS_H

# include <pthread.h>
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <semaphore.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <signal.h>

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
	sem_t				*print;
	sem_t				*update_tbc;
	sem_t				*update_dead;
	sem_t				*must_eat_done;
	pthread_t			share_thread;
	int					share_dead;
}	t_share;

typedef struct s_philo
{
	int				index;
	int				finish_must_eat;
	int				eat_count;
	unsigned long	time_begin_count;
	pid_t			pid;
	sem_t			*forks;
	t_share			*share;
	pthread_t		thread;
}	t_philo;

unsigned long	get_time(void);
void			ft_usleep(unsigned long millisecond);
void			printf_mutex(t_philo *philo, char *msg);
int				printf_die_mutex(t_philo *philo, int i);
int				printf_must_est_mutex(t_philo **philo);
void			exit_free(t_philo **philo, t_share *share,
					t_arg arg, sem_t *forks);
void			*counter(t_philo *philo);
void			*share_counter(t_philo **philo);
void			*activites(t_philo *philo);
void			init_philo(t_philo **philo, t_share *share, sem_t *forks);
void			init(t_philo **philo, t_share *share, t_arg arg, sem_t *forks);
int				ft_atoi(const char *str);
int				parser(int ac, char **av, t_arg *arg);

#endif
