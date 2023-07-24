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
#include <sys/time.h>

// Define the structure for a philosopher
typedef struct s_data
{
	int	number_of_philosophers;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	number_of_times_each_philo_must_eat;
}	t_data;

typedef struct s_philosopher 
{
	t_data				*data;
	int					philosopher_id;
	pthread_mutex_t		*left_fork;
	pthread_mutex_t		*right_fork;
	int					time_to_eat;
	int					time_to_sleep;
	int					number_of_times_to_eat;
	pthread_mutex_t		*forks_to_eat_mutex;
	int					*forks_to_eat;
}	t_philosopher;

// Define the function prototypes
void	*philosopher_action(void *arg);
void	pick_forks(t_philosopher *philosopher);
void	release_forks(t_philosopher *philosopher);
void	print_action(int philosopher_id, char *action);

int	main(int argc, char **argv)
{
	t_data	input_data;

	if (argc < 6)
	{
		printf("Invalid number of arguments.\n");
		printf("Usage: ./philosophers <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> <number_of_times_each_philo_must_eat>\n");
		return 1;
	}

	input_data.number_of_philosophers = atoi(argv[1]);
	input_data.time_to_die = atoi(argv[2]);
	input_data.time_to_eat = atoi(argv[3]);
	input_data.time_to_sleep = atoi(argv[4]);
	input_data.number_of_times_each_philo_must_eat = atoi(argv[5]);

	// Allocate memory for philosophers and forks
	t_philosopher	*philosophers;
	pthread_t		*philosopher_threads;
	pthread_mutex_t	*forks;
	int				*forks_to_eat;
	pthread_mutex_t	forks_to_eat_mutex;
	int i = 0;

	philosophers = malloc(input_data.number_of_philosophers * sizeof(t_philosopher));
	philosopher_threads = malloc(input_data.number_of_philosophers * sizeof(pthread_t));
	forks = malloc(input_data.number_of_philosophers * sizeof(pthread_mutex_t));
	forks_to_eat = malloc(input_data.number_of_philosophers * sizeof(int));

	// Initialize the forks and forks_to_eat_mutex
	i = 0;
	while (i < input_data.number_of_philosophers)
	{
		pthread_mutex_init(&forks[i], NULL);
		forks_to_eat[i] = input_data.number_of_times_each_philo_must_eat;
		i++;
	}
	pthread_mutex_init(&forks_to_eat_mutex, NULL);

	// Create the philosopher threads
	i = 0;
	while (i < input_data.number_of_philosophers)
	{
		philosophers[i].philosopher_id = i + 1;
		philosophers[i].left_fork = &forks[i];
		philosophers[i].right_fork = &forks[(i + 1) % input_data.number_of_philosophers];
		philosophers[i].time_to_eat = input_data.time_to_eat;
		philosophers[i].time_to_sleep = input_data.time_to_sleep;
		philosophers[i].number_of_times_to_eat = input_data.number_of_times_each_philo_must_eat;
		philosophers[i].forks_to_eat_mutex = &forks_to_eat_mutex;
		philosophers[i].forks_to_eat = forks_to_eat;

		pthread_create(&philosopher_threads[i], NULL, philosopher_action, &philosophers[i]);
		i++;
	}

	// Wait for all philosopher threads to finish
	i = 0;
	while (i < input_data.number_of_philosophers)
	{
		pthread_join(philosopher_threads[i], NULL);
		i++;
	}

	// Cleanup and free memory
	i = 0;
	while (i < input_data.number_of_philosophers)
	{
		pthread_mutex_destroy(&forks[i]);
	}
	pthread_mutex_destroy(&forks_to_eat_mutex);

	free(philosophers);
	free(philosopher_threads);
	free(forks);
	free(forks_to_eat);

	return 0;
}

void* philosopher_action(void* arg)
{
	t_philosopher	*philosopher = (t_philosopher *)arg;
	int 			philosopher_id = philosopher->philosopher_id;
	//int time_to_die = atoi(argv[2]);
	int				time_to_die = philosopher->data->time_to_die;
	struct timeval	current_time;
	int				forks_to_eat;

	while (1)
	{
		pick_forks(philosopher);
		print_action(philosopher_id, "is eating");
		usleep(philosopher->time_to_eat * 1000); // Sleep in microseconds
		
		// Update philosopher's state
		gettimeofday(&current_time, NULL);
		
		pthread_mutex_lock(philosopher->forks_to_eat_mutex);
		philosopher->forks_to_eat--;
		forks_to_eat = *philosopher->forks_to_eat;
		pthread_mutex_unlock(philosopher->forks_to_eat_mutex);
		
		if (time_to_die > 0 && forks_to_eat <= 0)
		{
			print_action(philosopher_id, "died");
			break;
		}
		
		release_forks(philosopher);
		print_action(philosopher_id, "is sleeping");
		usleep(philosopher->time_to_sleep * 1000); // Sleep in microseconds
		print_action(philosopher_id, "is thinking");

		// Check if all philosophers have eaten enough
		if (forks_to_eat <= 0)
		{
			break;
		}
	}
	return NULL;
}

void	pick_forks(t_philosopher* philosopher)
{
	pthread_mutex_lock(philosopher->left_fork);
	print_action(philosopher->philosopher_id, "has taken a fork");
	pthread_mutex_lock(philosopher->right_fork);
	print_action(philosopher->philosopher_id, "has taken a fork");
}

void	release_forks(t_philosopher* philosopher)
{
	pthread_mutex_unlock(philosopher->left_fork);
	pthread_mutex_unlock(philosopher->right_fork);
	print_action(philosopher->philosopher_id, "has released the forks");
}

void	print_action(int philosopher_id, char* action)
{
	struct timeval current_time;
	gettimeofday(&current_time, NULL);
	long timestamp = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	printf("(%ld ms) Philosopher %d %s\n", timestamp, philosopher_id, action);
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
