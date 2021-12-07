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
	sem_wait(philo->share->print);
	if (philo->share->share_dead == 0)
		printf("%ld\tphilo %d %s\n",
			get_time() - philo->share->start_time, philo->index, msg);
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
