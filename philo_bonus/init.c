#include "philosophers_bonus.h"

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


