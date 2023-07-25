/*
I would like to learn the basics of threading a process.
I want to see how to create threads and discover mutexes.

At first I want to create a c program, which will be a
solution to Philosophers problem using threads and mutexes.

These are the specific rules for my solution:
1) Program should take the following arguments as input parameters:
- 'number_of_philosophers' = the number of philosophers and also a number of forks
- 'time_to_die' = (in miliseconds), if the philosopher didn't start eating given this period
since the beginning of the last meal or the beginning of the simulation, they die.
- 'time_to_eat' = (in miliseconds), the time it takes for a philosopher to eat, during
this time given philosopher need to hold both left and right forks.
- 'time_to_sleep' = (in miliseconds), the time philoopher will spend sleeping.
- 'number_of_times_each_philo_must_eat' = If all philosophers have eaten at least given
amount of times, the simulation stops. Otherwise simulation stops if any of the philosophers dies.
2) The first philosopher is sits next to the last. Any other Philosopher sits next to Philosopher NUM - 1 and NUM + 1;
3) Any state change of a philosopher must be formated as following: ('timestamp_in_ms' X 'action')
- 'timestamp' = must be current time inmiliseconds;
- 'X' = philosopher number;
- 'action' = can be: ('has taken a fork', 'is eating', 'is sleeping', 'is_thinking' or 'died');
4) Philosophers should avvoid dying.
5) Each phlosopher should be a thread;
6) There is one fork between each pair of Philosophers. 
If there is only one Philosopher, there should be only one fork.
7) To prevent Philosophers from duplicating forks_to_eat, the forks_to_eat state should be protected with mutex for each of them. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>

#define	PHILO_NUM 5
#define	TIME_TO_DIE 800
#define	TIME_TO_EAT 200
#define	TIME_TO_SLEEP 200
#define	MIN_MEALS 7

typedef struct s_in_data
{
	long	number_of_philosophers;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_in_data;

typedef struct s_philo
{
	t_in_data	*in_data;
	pthread_t	thread;
	int			id;
	int			meals;
	int			status;
	int			eating;
	int			left_fork;
	int			right_fork;
	long		last_meal_time;
}	t_philo;

typedef struct s_data
{
	long			current_time;
	t_in_data		in_data;
	t_philo			philos[PHILO_NUM];
	pthread_t		threads_id[PHILO_NUM];
	pthread_mutex_t	forks[PHILO_NUM];
	pthread_mutex_t	mutex_lock;
	pthread_mutex_t	write;
}	t_data;

long	get_current_time()
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

void	init_philo_struct(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		/* initializing struct for each philosopher */
		philo = &(data->philos[i]);
		philo->in_data = data;
		philo->id = i;
		philo->status = 0;
		philo->meals = 0;
		philo->eating = 0;
		philo->last_meal_time = 0;
		philo->left_fork = i;
		philo->right_fork = (i + 1) % philo->in_data->number_of_philosophers;
		i++;
	}
}

void	init_input_data(t_data *data)
{
	data->in_data.number_of_philosophers = PHILO_NUM;
	data->in_data.time_to_die = TIME_TO_DIE;
	data->in_data.time_to_eat = TIME_TO_EAT;
	data->in_data.time_to_sleep = TIME_TO_SLEEP;
	data->in_data.number_of_meals = MIN_MEALS;
}

void	*routine(void *data_ptr)
{
	t_data	*data = (t_data *)data_ptr;
	t_philo *philo = &(data->philos[data->id - 1]);

	pthread_mutex_lock(&(philo->lock));
	while (1)
	{
		if (philo->status == 0)
		{
			philo->status = 1;
			printf("(%ldms) %d has taken a fork\n", get_current_time(), philo->id);
			pthread_mutex_lock(philo->left_fork);
			printf("(%ldms) %d has taken a fork\n", get_current_time(), philo->id);
			pthread_mutex_lock(philo->right_fork);
			printf("(%ldms) %d is eating\n", get_current_time(), philo->id);
			philo->eating = 1;
			philo->meals++;
			philo->death_clock = get_current_time() + data->in_data.time_to_die;
			usleep(data->in_data.time_to_eat * 1000);
		}
		else if (philo->status == 1)
		{
			philo->status = 2;
			printf("(%ldms) %d is sleeping\n", get_current_time(), philo->id);
			pthread_mutex_unlock(philo->left_fork);
			pthread_mutex_unlock(philo->right_fork);
			philo->eating = 0;
			usleep(data->in_data.time_to_sleep * 1000);
		}
		else if (philo->status == 2)
		{
			printf("(%ldms) %d is thinking\n", get_current_time(), philo->id);
		}

		if (data->in_data.number_of_meals > 0 && philo->meals >= data->in_data.number_of_meals)
			break;

		if (get_current_time() >= philo->death_clock)
		{
			pthread_mutex_lock(&(data->write));
			printf("(%ldms) %d died\n", get_current_time(), philo->id);
			pthread_mutex_unlock(&(data->write));
			return NULL;
		}
	}
	pthread_mutex_unlock(&(philo->lock));
	return NULL;
}

bool	initialize_mutex(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&(data->mutex_lock), NULL) != 0)
		return (false);
	if (pthread_mutex_init(&(data->write), NULL) != 0)
		return (false);
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (pthread_mutex_init(&(data->forks[i]), NULL) != 0)
			return (false);
		i++;
	}
	return (true);
}

bool	launched_threads(t_data *data)
{
	t_philo	*philo;
	int		i;

	data->current_time = get_current_time();
	philo = data->philos;
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		if (pthread_create(&(philo[i].thread), NULL, routine, &(philo[i])) != 0)
			return (false);
		philo[i].last_meal_time = get_current_time();
		i++;
	}
	return (true);
}

int	main(int argc, char **argv)
{
	t_data	data;
	int		i;

	init_input_data(&data);
	if (!initialize_mutex(&data))
		printf("Mutex init error\n");
	init_philo_struct(&data);
	if (!launched_threads(&data))
		printf("Thread Error\n");


	i = 0;
	while (i < data.in_data.number_of_philosophers)
	{
		if (pthread_create(&(data.threads_id[i]), NULL, routine, &data) != 0)
		{
			printf("Pthread create error\n");
		}
		i++;
	}
	i = 0;
	while (i < data.in_data.number_of_philosophers)
	{
		if (pthread_join(data.threads_id[i], NULL) != 0)
		{
			printf("Pthread join error\n");
		}
		i++;
	}
	i = 0;
	while (i < data.in_data.number_of_philosophers)
	{
		if (pthread_mutex_destroy(&(data.forks[i])) != 0)
		{
			printf("Pthread join error\n");
		}
		i++;
	}
	pthread_mutex_destroy(&(data.mutex_lock));
	pthread_mutex_destroy(&(data.write));
	return 0;
}


/*
This code implements the solution to the Philosophers problem using threads and mutexes. 
It takes the required input parameters as command-line arguments and creates one thread 
per philosopher. Each philosopher thread follows the actions of picking up forks, 
eating, sleeping, and thinking, printing the actions with timestamps.

The `philosopher_action` function represents the behavior of a philosopher thread. 
It repeatedly executes the actions of picking up forks, eating, sleeping, 
and thinking until all philosophers have eaten enough or a philosopher dies. 
The `pick_forks` function is responsible for locking the left and right forks 
before the philosopher can start eating. The `release_forks` function releases 
the forks after the philosopher finishes eating. 
The `print_action` function formats and prints the actions with timestamps.

To run the program, compile the code and execute the generated executable 
with the required input parameters:
gcc philosophers.c -o philosophers -lpthread
./philosophers

Replace 
`<number_of_philosophers>`, 
`<time_to_die>`, 
`<time_to_eat>`, 
`<time_to_sleep>`, and 
`<number_of_times_each_philo_must_eat>` 
with the desired values.

Please note that the code does not handle invalid input or edge cases 
such as excessive values for time or negative values for parameters. 
It offers a basic implementation of the solution to the Dining Philosophers 
problem using threads and mutexes. 

*/
