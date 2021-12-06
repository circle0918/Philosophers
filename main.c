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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
typedef struct s_arg
{
	int total;
	int die;
	int eat;
	int sleep;
	int must_eat;
}	t_arg;

typedef struct s_share
{
	t_arg			arg;
	unsigned long		start_time;
	pthread_mutex_t	printf_mutex;
	pthread_mutex_t	somebody_dead_mutex;
	pthread_t		share_thread;
	int				share_dead;
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
	t_share			*share;
}	t_philo;

int	ft_atoi(const char *str)
{
	int		sign;
	long	ret;

	ret = 0;
	sign = 1;
	while ((*str >= 9 && *str <= 13) || *str == 32)
		++str;
	if (*str == '+' || *str == '-')
		if (*(str++) == '-')
			sign *= -1;
	while (*str >= '0' && *str <= '9')
	{
		ret = ret * 10 + sign * (*str++ - '0');
		if (ret > 2147483647)
			return (-1);
		else if (ret < -2147483648)
			return (0);
	}
	return ((int)ret);
}

int isnum(int ac, char **av)
{
	int i;
	int j;

	i = 1;
	j = 0;
	while(i < ac)
	{
		while(av[i][j])
		{
			if(!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (-1);
			j++;
		}
		i++;
	}
	return (1);
}

int parser(int ac, char **av, t_arg *arg)
{
	if((ac == 5 || ac == 6) && (isnum(ac, av)))
	{
		arg->total = ft_atoi(av[1]);
		arg->die = ft_atoi(av[2]);
		arg->eat = ft_atoi(av[3]);
		arg->sleep = ft_atoi(av[4]);
		if (ac == 6)
			arg->must_eat = ft_atoi(av[5]);
		else
			arg->must_eat = 2147483647;
		if(arg->total < 0|| arg->die < 0 || arg->eat < 0 || arg->sleep < 0 || arg->must_eat < 0)
		{
			printf("%s\n", "Error argument : wrong input");
			return (1);
		}
	}
	else
	{	
		printf("%s\n", "Error argument : wrong input");
		return (1);
	}
	return (0);
}

//mille second 
unsigned long get_time()
{
	struct timeval t;
 	
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

void	printf_mutex(t_philo *philo, char* msg)
{
	pthread_mutex_lock(&philo->share->printf_mutex);
	if(philo->share->share_dead == 0)
		printf("%ld\tphilo %d %s\n", get_time() - philo->share->start_time, philo->index, msg);
	pthread_mutex_unlock(&philo->share->printf_mutex);
}

void   *printf_die_mutex(t_philo **philo, int i)
{
	philo[i]->share->share_dead = 1;
	pthread_mutex_lock(&philo[0]->share->printf_mutex);
	printf("%ld\tphilo %d die\n", get_time() - philo[i]->share->start_time, philo[i]->index);
	pthread_mutex_unlock(&philo[0]->share->printf_mutex);
	pthread_mutex_unlock(&(philo[0]->share->somebody_dead_mutex));
	return (NULL);
}

void	*printf_must_est_mutex(t_philo **philo, int i)
{			
	philo[0]->share->share_dead = 1;
	pthread_mutex_lock(&philo[0]->share->printf_mutex);
	printf("must eat done\n");
	pthread_mutex_unlock(&philo[0]->share->printf_mutex);
	pthread_mutex_unlock(&(philo[0]->share->somebody_dead_mutex));	
	return (NULL);
}

void *share_counter(t_philo **philo)
{
	int i;
	int must_eat_done;
	int finish_must_eat;

	while (1)
	{
		i = 0;
		must_eat_done = 1;
		while(i < philo[0]->share->arg.total)
		{
			if ((int)(get_time() - philo[i]->time_begin_count) > philo[i]->share->arg.die)
				return (printf_die_mutex(philo, i));

			finish_must_eat = philo[i]->eat_count >= philo[i]->share->arg.must_eat;
			must_eat_done = must_eat_done && finish_must_eat; 
			i++;
		}
		if (must_eat_done)
			return (printf_must_est_mutex(philo, i));
		ft_usleep(1);
	}
	return (NULL);
}

void *activites(t_philo *philo)
{
	while(1)
	{
		pthread_mutex_lock(philo->lfork);
		printf_mutex(philo, "has taken a lfork");
		pthread_mutex_lock(philo->rfork);
		printf_mutex(philo, "has taken a rfork");
		philo->time_begin_count = get_time();
		printf_mutex(philo, "is eating");
		//usleep(philo->share->arg.eat * 1000);
		ft_usleep(philo->share->arg.eat);
		pthread_mutex_unlock(philo->lfork); 
		pthread_mutex_unlock(philo->rfork);
		philo->eat_count++;
		printf_mutex(philo, "is sleeping");
		//usleep(philo->share->arg.sleep * 1000);
		ft_usleep(philo->share->arg.sleep);
		printf_mutex(philo, "is thinking");
	}
	return (NULL);
}

void init_philo(t_philo **philo, t_share *share, pthread_mutex_t **fork, int i)
{
	while(i < share->arg.total)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->share = share;
		philo[i]->lfork = fork[i];
		philo[i]->time_begin_count = share->start_time;
		philo[i]->eat_count = 0;
		if(i == share->arg.total - 1)
			philo[i]->rfork = fork[0];
		else
			philo[i]->rfork = fork[i+1];
		philo[i]->index = i + 1;
	    pthread_create(&(philo[i]->thread), NULL, (void *)&activites, philo[i]);
		pthread_detach(philo[i]->thread);
		i = i + 2;
		usleep(20);
	}
}

void init(t_philo **philo, t_share *share, t_arg arg, pthread_mutex_t **fork)
{
	int i;

	share->arg = arg;
	share->share_dead = 0;
	share->start_time = get_time();
	pthread_mutex_init(&(share->somebody_dead_mutex), NULL);
	pthread_mutex_lock(&(share->somebody_dead_mutex));
	pthread_mutex_init(&share->printf_mutex, NULL);

	i = 0;
	while(i < arg.total)
	{
		fork[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
		i++;
	}
	init_philo(philo, share, fork, 0);
	ft_usleep(50);
	init_philo(philo, share, fork, 1);
	pthread_create(&(share->share_thread), NULL, (void *)&share_counter, philo);
	pthread_detach(share->share_thread);
}

void exit_free(t_philo **philo, t_share *share, t_arg arg, pthread_mutex_t **fork)
{
	pthread_mutex_destroy(&(share->printf_mutex));
	pthread_mutex_destroy(&(share->somebody_dead_mutex));
	int i = 0;
	while (i < arg.total)	
	{
		pthread_mutex_destroy(fork[i]);
		free(philo[i]);
		free(fork[i]);
		i++;
	}
	free(fork);
	free(philo);
}

int main(int ac, char **av)
{
	t_arg 	arg;
	t_philo **philo;
	pthread_mutex_t **fork;
	t_share share;

	if (parser(ac, av, &arg))
		return (0);
	philo = malloc(sizeof(t_philo) * arg.total);
	fork = malloc(sizeof(pthread_mutex_t) * arg.total);
	if(!philo || !fork)
		return (-1);
	init(philo, &share, arg, fork);
	pthread_mutex_lock(&share.somebody_dead_mutex);
	exit_free(philo, &share, arg, fork);
	return (0);
}
