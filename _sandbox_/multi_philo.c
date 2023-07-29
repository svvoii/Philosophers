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
#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/time.h>
#include <time.h>

#define	PHILO_NUM 5
#define	TIME_TO_DIE 800
#define	TIME_TO_EAT 200
#define	TIME_TO_SLEEP 200
#define	MIN_MEALS 7

enum	e_state
{
	NOT_ALIVE,
	HUNGRY,
	EATING,
	SLEEPING,
	THINKING
};

/* this holds the initial input data */
typedef struct s_in_data
{
	long	number_of_philosophers;
	long	time_to_die;
	long	time_to_eat;
	long	time_to_sleep;
	long	number_of_meals;
}	t_in_data;

/* this data reflects the current status of the philo */
typedef struct s_status
{
	enum e_state	state;
	int				meals;
	int64_t			last_meal_time;
	int64_t			next_meal_time;
}	t_status;

typedef struct s_philo
{
	pthread_t		thread;
	int				id;
	int				left_fork;
	int				right_fork;
	t_status		status;
	struct s_data	*data;
	pthread_mutex_t	mutex_print_log;
}	t_philo;

typedef struct s_data
{
	int64_t			start_time;
	t_in_data		in_data;
	t_philo			philos[PHILO_NUM];
	pthread_mutex_t	forks[PHILO_NUM];
	//pthread_mutex_t	mutex_lock;
	pthread_mutex_t	mutex_print_log;
}	t_data;

long	get_current_time()
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000) + (tp.tv_usec / 1000));
}

int64_t	timestamp(t_data *data)
{
	int64_t	current_time;
	int64_t	timestamp;

	current_time = get_current_time();
	return (current_time - data->start_time);
}

void	print_log(t_philo *philo, char *message)
{
	t_data	*data;

	data = philo->data;
	pthread_mutex_lock(&(philo->mutex_print_log));
	printf("[%ld] Philo [%d] %s\n", timestamp(data), philo->id, message);
	pthread_mutex_unlock(&(philo->mutex_print_log));
}

void	init_philo_struct(t_data *data)
{
	t_philo	*philo;
	long	total_philo;
	int		i;

	printf("..init_philo..\n");
	data->start_time = get_current_time();
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		/* initializing struct for each philosopher */
		philo = &(data->philos[i]);
		total_philo = data->in_data.number_of_philosophers;
		philo->id = i;
		philo->left_fork = i;
		philo->right_fork = (i + 1) % total_philo;
		philo->status.state = HUNGRY;
		philo->status.meals = 0;
		philo->status.last_meal_time = data->start_time;
		philo->status.next_meal_time = data->start_time + data->in_data.time_to_die;
		philo->data = data;
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

bool	initialize_mutex(t_data *data)
{
	int	i;

	if (pthread_mutex_init(&(data->mutex_print_log), NULL) != 0)
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

void	*routine(void *philo_ptr);

bool	launched_threads(t_data *data)
{
	t_philo	*philo;
	int		i;

	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		philo = &(data->philos[i]);
		if (pthread_create(&(philo->thread), NULL, &routine, philo) != 0)
			return (false);
		i++;
	}
	/* might need to create additional thread for monitoring whether philos are alive */
	return (true);
}

int	main(int argc, char **argv)
{
	t_data		data;
	int			i;

	init_input_data(&data);
	if (!initialize_mutex(&data))
		printf("Mutex init error\n");
	//printf("Mutex init OK\n");
	init_philo_struct(&data);
	if (!launched_threads(&data))
		printf("Thread Error\n");

	i = 0;
	while (i < data.in_data.number_of_philosophers)
	{
		if (pthread_join(data.philos[i].thread, NULL) != 0)
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
			printf("Pthread mutex destroy error\n");
		}
		i++;
	}
	pthread_mutex_destroy(&(data.mutex_print_log));
	return 0;
}

void	ft_usleep(int ms)
{
	int64_t	time;

	time = get_current_time();
	usleep(ms * 920);
	while (get_current_time() < time + ms)
	{
		usleep(ms * 3);
	}
}

/* This will monitor whether all philos are still alive, inside routine */
bool	philosophers_alive(t_philo *philo)
{
	t_data	*data;
	t_philo	*philo_tmp;
	int64_t	current_time;
	int		i;

	data = philo->data;
	i = 0;
	while (i < data->in_data.number_of_philosophers)
	{
		philo_tmp = &data->philos[i];
		current_time = get_current_time();
		if (current_time > philo_tmp->status.next_meal_time)
		{
			print_log(philo_tmp, "is no longer alive :(");
			//printf("[%ld]\tPhilo [%d] is no longer alive :(\n", timestamp(data), philo_tmp->id);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	available_meals(t_philo *philo)
{
	t_data	*data;
	t_philo	*philo_tmp;
	int		i;

	data = philo->data;
	if (data->in_data.number_of_meals > 0)
	{
		i = 0;
		while (i < data->in_data.number_of_philosophers)
		{
			philo_tmp = &data->philos[i];
			if (philo_tmp->status.meals >= data->in_data.number_of_meals)
				return (false);
			i++;
		}
	}
	return (true);
}

bool pickup_forks(t_philo *philo)
{
    t_data	*data = philo->data;
	int		attempt;

    //pthread_mutex_lock(&(data->forks[philo->left_fork]));
    //pthread_mutex_lock(&(data->forks[philo->right_fork]));

	attempt = pthread_mutex_trylock(&(data->forks[philo->left_fork]));
	if (attempt != 0)
	{
		print_log(philo, "attempted to pick up LEFT fork but its locked..");
		return (false);
	}
	else
		print_log(philo, "picked up LEFT fork");
	attempt = pthread_mutex_trylock(&(data->forks[philo->right_fork]));
	if (attempt != 0)
	{
		print_log(philo, "attempted to pick up RIGHT fork but its locked..");
		pthread_mutex_unlock(&(data->forks[philo->left_fork]));
		return (false);
	}
	else
		print_log(philo, "picked up RIGHT fork");
	return (true);
}

void eating(t_philo *philo)
{
    t_data *data = philo->data;

    philo->status.meals++;
    philo->status.last_meal_time = get_current_time();
    philo->status.next_meal_time = philo->status.last_meal_time + data->in_data.time_to_die;

    ft_usleep(data->in_data.time_to_eat);

    pthread_mutex_unlock(&(data->forks[philo->left_fork]));
    pthread_mutex_unlock(&(data->forks[philo->right_fork]));
}

void sleeping(t_philo *philo)
{
    t_data *data = philo->data;

    ft_usleep(data->in_data.time_to_sleep);
}

void thinking(t_philo *philo)
{
    philo->status.state = HUNGRY;
}

void	*routine(void *philo_ptr)
{
	t_philo	*philo;
	long	total_philo;

	philo = (t_philo *)philo_ptr;
	total_philo = philo->data->in_data.number_of_philosophers;

	/*
	while (1)
	{
		if (!ft_eat())
			break ;
		if (!ft_sleep())
			break ;
		if (!ft_think())
			break ;
	}
	return (NULL);
	*/
	while (philosophers_alive(philo) && available_meals(philo))
	{
		/*
		piking_up_fork(philo);
		eat(philo);
		sleep(philo);
		think(philo);
		*/
		if (philo->status.state == HUNGRY)
		{
			print_log(philo, "is HUNGRY");
			if (pickup_forks(philo))
				philo->status.state = EATING;
		}
		if (philo->status.state == EATING)
		{
			print_log(philo, "is EATING");
			eating(philo);
			philo->status.state = SLEEPING;
		}
		if (philo->status.state == SLEEPING)
		{
			print_log(philo, "is SLIPING");
			sleeping(philo);
			philo->status.state = THINKING;
		}
		if (philo->status.state == THINKING)
		{
			print_log(philo, "is THINKING");
			thinking(philo);
			//philo->status.state = HUNGRY;
		}
	}
}

/* the function to monitore if the philos are alive */
/*
void	*life_monitor(void *data_ptr)
{
	printf("\tlife_monitor\tdata @ [%p]\n", data_ptr);
}

void	*routine(void *philo_ptr)
{
	t_philo *philo = (t_philo *)philo_ptr;

	pthread_mutex_lock(&(philo->data->mutex_lock));
	while (1)
	{
		if (philo->status.state == HUNGRY)
		{
			philo->status.state = EATING;
			printf("(%ldms) %d has taken a fork\n", get_current_time(), philo->id);
			pthread_mutex_lock(&(philo->data->forks[philo->left_fork]));
			printf("(%ldms) %d has taken a fork\n", get_current_time(), philo->id);
			pthread_mutex_lock(&(philo->data->forks[philo->right_fork]));
			printf("(%ldms) %d is eating\n", get_current_time(), philo->id);
			philo->status.meals++;
			philo->status.last_meal_time = get_current_time();
			philo->status.next_meal_time = get_current_time() + philo->data->in_data.time_to_die;
			usleep(philo->data->in_data.time_to_eat * 1000);
			pthread_mutex_unlock(&(philo->data->forks[philo->left_fork]));
			pthread_mutex_unlock(&(philo->data->forks[philo->right_fork]));
			philo->status.state = TIRED;
		}
		else if (philo->status.state == TIRED)
		{
			philo->status.state = SLEEPING;
			printf("(%ldms) %d is sleeping\n", get_current_time(), philo->id);
			usleep(philo->data->in_data.time_to_sleep * 1000);
			philo->status.state = THINKING;
		}
		else if (philo->status.state == THINKING)
		{
			printf("(%ldms) %d is thinking\n", get_current_time(), philo->id);
		}
		if (philo->data->in_data.number_of_meals > 0 && philo->status.meals >= philo->data->in_data.number_of_meals)
			break;

		if (get_current_time() >= philo->status.next_meal_time)
		{
			pthread_mutex_lock(&(philo->data->write));
			printf("(%ldms) %d died\n", get_current_time(), philo->id);
			pthread_mutex_unlock(&(philo->data->write));
			return NULL;
		}
	}
	pthread_mutex_unlock(&(philo->data->mutex_lock));
	return NULL;
}
*/

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

*********
There is no synchronization in accessing shared resources like forks or print statements. 
This can lead to race conditions and undefined behavior. Mutexes should be used to protect critical 
sections of code where shared resources are accessed.

There is no synchronization between the threads. The code doesn't ensure that the threads run in a 
coordinated manner. This can lead to inconsistent behavior and unexpected results. 
Synchronization mechanisms like mutexes or condition variables should be used to coordinate the actions 
of multiple threads.
*********
*/
