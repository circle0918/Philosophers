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
#include <stdlib.h>
typedef struct s_arg
{
	int total;
	int die;
	int eat;
	int sleep;
	int must_eat;
}	t_arg;

typedef struct s_philo
{
	pthread_t		thread;
	pthread_mutex_t	*lfork;
	pthread_mutex_t	*rfork;
	t_arg			arg;
	
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
			arg->must_eat = -1;
		if(arg->total < 0|| arg->die < 0 || arg->eat < 0 || arg->sleep < 0 )
		{
			printf("%s\n", "Error argument : wrong input");
			return (1);
		}
	
//		printf("%d\n", philo->arg.total);
//		printf("%d\n", philo->arg.must_eat);
	}
	else
	{	
		printf("%s\n", "Error argument : wrong input");
		return (1);
	}
	return (0);
}
//pthread_mutex_t	fork;
void fun1(t_philo *philo)
{
//	int res = pthread_mutex_lock(&fork);
	int i = 0;
	while(i < 1)
	{
		printf("id : %d lfork : %p rfork:  %p\n", philo->thread, philo->lfork, philo->rfork);
	//	sleep(1);
		i++;
	}
  //  pthread_mutex_unlock(&fork); 
}
void init(t_philo **philo, t_arg arg, pthread_mutex_t **fork)
{
	int i;
	int j;

	i = 0;
	j = arg.total;
	
	while(i < j)
	{
		
		fork[i] = malloc(sizeof(pthread_mutex_t));
		pthread_mutex_init(fork[i], NULL);
		i++;
	}
	i = 0;
	while(i < j)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->arg = arg;
		philo[i]->lfork = fork[i];
		if(i == j - 1)
			philo[i]->rfork = fork[0];
		else
			philo[i]->rfork = fork[i+1];
	    pthread_create(&(philo[i]->thread), NULL, (void *)&fun1, philo[i]);
		i++;
	}

	//printf("%d\n",philo[2]->arg.total);
}
int main(int ac, char **av)
{
/*	t_philo	philo1;
	t_philo	philo2;*/
	t_arg 	arg;
	t_philo **philo;
	pthread_mutex_t **fork;

	if (parser(ac, av, &arg))
		return (0);
	philo = malloc(sizeof(t_philo) * arg.total);
	fork = malloc(sizeof(pthread_mutex_t) * arg.total);
	if(!philo || !fork)
		return (-1);
	init(philo, arg, fork);
//	philo1.arg = arg; //while (init all);
//	printf("%d\n", philo1.arg.eat);
	int i = 0;
	while(i < arg.total)	
	{	
		pthread_join(philo[i]->thread, NULL);
		i++;
	}
	//	pthread_join(philo2.thread,NULL);*/
		
	i = 0;
	while(i < arg.total)	
	{
		free(fork[i]);
		free(philo[i]);
		i++;
	}
		free(fork);
		free(philo);
	
//	printf("%d", ft_atoi(av[1]));
	return (0);
}
