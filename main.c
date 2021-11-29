/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yyuan <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/29 12:08:24 by yyuan             #+#    #+#             */
/*   Updated: 2021/11/29 13:37:25 by yyuan            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include <pthread.h>
#include <stdio.h>
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
	pthread_t		thread_id;
	pthread_mutex_t	lfork;
	pthread_mutex_t	rfork;
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
	while(i<ac)
	{
		while(av[i][j])
		{
			if(!(av[i][j] >= '0' && av[i][j] <= '9'))
				return (-1);
			j++;
		}
		i++;
	}
}
void parser(int ac, char **av, t_philo *philo)
{
	if((ac == 5 || ac == 6) && !(isnum(ac, av)))
	{
		philo->arg.total = ft_atoi(av[1]);
		philo->arg.die = ft_atoi(av[2]);
		philo->arg.eat = ft_atoi(av[3]);
		philo->arg.sleep = ft_atoi(av[4]);
		if (ac == 6)
			philo->arg.must_eat = ft_atoi(av[5]);
		else
			philo->arg.must_eat = -1;
	
		printf("%d\n", philo->arg.total);
		printf("%d\n", philo->arg.must_eat);
	}
	else
		printf("%s\n", "Error : argument");
}
int main(int ac, char **av)
{
	t_philo	philo;
//	printf("%d", ft_atoi(av[1]));
	parser(ac, av, &philo);
	return (0);
}
