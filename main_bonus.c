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

#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>

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
	sem_t	*print;
	sem_t	*must_eat_done;
	pthread_t		share_thread;
	int				share_dead;
}	t_share;

typedef struct s_philo
{
	int				index;
	int				finish_must_eat;
	int				eat_count;
	unsigned long	time_begin_count;
	pid_t		pid;
	sem_t	*forks;
	t_share			*share;
	pthread_t		thread;
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
	sem_wait(philo->share->print);
	if(philo->share->share_dead == 0)
		printf("%ld\tphilo %d %s\n", get_time() - philo->share->start_time, philo->index, msg);
	sem_post(philo->share->print);
}

int	printf_die_mutex(t_philo *philo, int i)
{
	philo->share->share_dead = 1;
	sem_wait(philo->share->print);
	printf("%ld\tphilo %d die\n", get_time() - philo->share->start_time, i);
	return (1);
}

int	printf_must_est_mutex(t_philo **philo)
{			
	philo[0]->share->share_dead = 1;
	sem_wait(philo[0]->share->print);
	printf("must eat done\n");
	return (1);
}

void exit_free(t_philo **philo, t_share *share, t_arg arg, sem_t *forks)
{
	int i = 0;
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
	sem_unlink("forks");
	sem_unlink("must_eat_done");
	sem_unlink("print");
}

void *counter(t_philo *philo)
{
	while (1)
	{
		if ((int)(get_time() - philo->time_begin_count) > philo->share->arg.die)
			exit (printf_die_mutex(philo, philo->index));
		ft_usleep(1);
	}
	return (NULL);
}

void *share_counter(t_philo **philo)
{
	int i;

	i = 0;
	while(i < philo[0]->share->arg.total)
	{
		sem_wait(philo[0]->share->must_eat_done);
		i++;
	}
	//exit_free(philo, philo[0]->share, philo[0]->share->arg, philo[0]->forks);
	exit(printf_must_est_mutex(philo));
	return (NULL);
}

void *activites(t_philo *philo)
{
	while(1)
	{
		sem_wait(philo->forks);
		printf_mutex(philo, "has taken a lfork");
		printf_mutex(philo, "has taken a rfork");
		philo->time_begin_count = get_time();
		printf_mutex(philo, "is eating");
		//usleep(philo->share->arg.eat * 1000);
		ft_usleep(philo->share->arg.eat);
		sem_post(philo->forks);
		philo->eat_count++;
		if (philo->eat_count == philo->share->arg.must_eat)
			sem_post(philo->share->must_eat_done);
		printf_mutex(philo, "is sleeping");
		//usleep(philo->share->arg.sleep * 1000);
		ft_usleep(philo->share->arg.sleep);
		printf_mutex(philo, "is thinking");
	}
	return (NULL);
}

void init_philo(t_philo **philo, t_share *share, sem_t *forks)
{
	int i;

	i = 0;
	while(i < share->arg.total)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->share = share;
		philo[i]->forks = forks;
		philo[i]->time_begin_count = share->start_time;
		philo[i]->eat_count = 0;
		philo[i]->index = i + 1;

printf("before fork! \n");
		philo[i]->pid = fork();
		if (philo[i]->pid == -1)
		{
			printf("Error: fork()\n");
		//	exit_free(philo, share, share->arg, forks);
			exit(1);
		}
		if (philo[i]->pid == 0)
		{
			pthread_create(&(philo[i]->thread), NULL, (void *)&counter, philo[i]);
			pthread_detach(philo[i]->thread);
			activites(philo[i]);
		}
		i++;	
	}
}

void init(t_philo **philo, t_share *share, t_arg arg, sem_t *forks)
{
	share->arg = arg;
	share->share_dead = 0;
	share->start_time = get_time();
	
	sem_unlink("print");
	share->print = sem_open("print", O_CREAT, S_IRWXU, 1);
	sem_unlink("must_eat_done");
	share->must_eat_done = sem_open("must_eat_done", O_CREAT, S_IRWXU, 0);

	init_philo(philo, share, forks);
	pthread_create(&(share->share_thread), NULL, (void *)&share_counter, philo);
	pthread_detach(share->share_thread);
}



int main(int ac, char **av)
{
	t_arg 	arg;
	t_philo **philo;
	sem_t *forks;
	t_share share;

	if (parser(ac, av, &arg))
		return (0);
	philo = malloc(sizeof(t_philo) * arg.total);
	sem_unlink("forks");
	forks = sem_open("forks", O_CREAT, S_IRWXU, arg.total / 2);
	if(!philo || !forks)
		return (-1);
	init(philo, &share, arg, forks);
	waitpid(-1, NULL, 0);
	exit_free(philo, &share, arg, forks);
	return (0);
}
