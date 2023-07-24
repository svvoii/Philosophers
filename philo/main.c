/*
MANDATORY PART

Program name: philo

Description: Philosophers with threads and mutexes

Argumnets:	number_of_philosophers 
			time_to_die
			time_to_eat
			time_to_sleep
			[number_of_times_each_philosopher_must_eat]

External functions:
			memset, printf, malloc, free, write, usleep, gettimeofday, pthread_create,
			pthread_detach, pthread_join, pthread_mutex_init, 
			pthread_mutex_destroy, pthread_mutex_lock, pthread_mutex_unlock

The specific rules for the mandatory part are:

• Each philosopher should be a thread.

• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.

• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.

=============================
BONUS PART

Program name: philo_bonus

Description: Philosophers with processes and semaphores

External functions:
			memset, printf, malloc, free, write, fork, kill, exit, pthread_create, 
			pthread_detach, pthread_join, usleep, gettimeofday, waitpid, 
			sem_open, sem_close, sem_post, sem_wait, sem_unlink

*/

int	main(int argc, char **argv)
{

	return (0);
}
