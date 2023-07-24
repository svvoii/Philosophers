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
/*
** WHAT IS MUTEX
**
pthread_mutex_t:
is a data type in the POSIX threads library (pthread) for managing mutual exclusion, 
or in other words, for preventing multiple threads from simultaneously accessing shared 
resources. It provides a way to enforce atomicity and ensure orderly access to critical 
sections of code.

pthread_mutex_t:
is a structure that represents a mutex, which stands for mutually exclusive. 
A mutex is a synchronization primitive used to protect shared resources, 
ensuring that only one thread can access the protected section at a time.

The (pthread_mutex_t) structure typically lives in memory, allocated either statically 
or dynamically, and its address is passed to pthread functions that require a mutex.

To create and initialize a mutex, you would declare a variable of type pthread_mutex_t 
and use the (pthread_mutex_init) function to initialize it. 
The mutex needs to be initialized before it can be used.
Once a mutex is initialized, threads can use it to lock and unlock the critical sections 
of code using (pthread_mutex_lock) and pthread_mutex_unlock functions, respectively.

When a thread calls (pthread_mutex_lock) on a mutex, it attempts to acquire the lock. 
If the lock is already held by another thread, the calling thread is blocked 
and put in a waiting state until the lock becomes available.

When a thread is done using the protected resource, it calls 
(pthread_mutex_unlock) 
to release the lock, allowing other waiting threads to acquire the lock.
The behavior of a mutex can be further customized by setting additional attributes 
using functions like 
(pthread_mutexattr_settype), 
(pthread_mutexattr_setpshared), etc.


It is important to ensure proper handling and error checking when working with mutexes. 
Functions like 
(pthread_mutex_trylock), 
(pthread_mutex_timedlock), and 
(pthread_mutex_destroy) 
are also available for more specific scenarios.

Mutexes are commonly used in multithreaded programs to protect shared resources 
like variables, data structures, files, or any section of code that should 
only be accessed by one thread at a time. 
They help prevent race conditions and ensure data integrity in concurrent environments.

When used correctly, mutexes provide a mechanism for safe and synchronized access 
to shared resources, allowing multiple threads to coexist and work cooperatively 
without interfering with each other.
*/

/*
Misuse of mutexes: 
The mutexes can be locked in a way that may result in a deadlock. 
The issue arises because each philosopher first locks the left fork and then 
tries to lock the right fork. 
If all philosophers pick up their left fork simultaneously, 
they will be locked indefinitely, unable to acquire their respective right forks.
To fix this, a suggestion is to modify the code to perform a "test and acquire" 
strategy where each philosopher attempts to acquire both forks but releases them 
if the right fork is unavailable. This approach will avoid potential deadlocks.

Using incorrect mutex variables: 
The mutexes used in the pthread_mutex_lock and pthread_mutex_unlock calls 
are not compatible with the ones declared in the forks_to_eat array. 
The forks_to_eat array uses pthread_mutex_t type, but the code uses pthread_mutex_t*. 
This discrepancy will lead to compilation errors.
To resolve this, update the declaration of 
forks_to_eat array to use pthread_mutex_t * type instead.

Missing error checking: The code lacks proper error checking after calling pthread_mutex_init. It is important to validate the return value of these functions to ensure that the mutexes are created successfully.
To address this, check the return value of pthread_mutex_init and handle any errors appropriately.


*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM 5

pthread_mutex_t forks_to_eat[NUM];

void	*philosopher(void *arg)
{
	int phil_id;
	int left_fork;
	int right_fork;
	int	result;

	phil_id = *(int *)arg;
	left_fork = phil_id;
	right_fork = (phil_id + 1) % NUM;

	// attempt to pick up left fork
	pthread_mutex_lock(&forks_to_eat[left_fork]);
	printf("Philosopher [%d] picks up LEFT fork [%d]\n", phil_id, left_fork);

	// attempt to pick up right fork
	//pthread_mutex_lock(&forks_to_eat[right_fork]);
	result = pthread_mutex_trylock(&forks_to_eat[right_fork]);
	if (result != 0)
	{
		printf("\tright_fork [%d] is locked, philo [%d] drops left_fork [%d]\n", right_fork, phil_id, left_fork);
		pthread_mutex_unlock(&forks_to_eat[left_fork]);
		return (NULL);
	}
	printf("Philosopher [%d] picks up RIGHT fork [%d]\n", phil_id, right_fork);

	// Philo is eating
	printf("Philosopher [%d] is eating\n", phil_id);

	// Philo releases left fork
	pthread_mutex_unlock(&forks_to_eat[left_fork]);
	printf("Philosopher [%d] releases fork [%d] (LEFT)\n", phil_id, left_fork);

	// Philosopher releases right fork
	pthread_mutex_unlock(&forks_to_eat[right_fork]);
	printf("Philosopher [%d] releases fork [%d] (RIGHT)\n", phil_id, right_fork);

	// Philosopher is thinking
	printf("Philosopher [%d] is thinking\n", phil_id);

	return NULL;
}

int main()
{
	pthread_t	philo_threads[NUM];
	int			philo_id[NUM];
	int			i;

	// Initialize mutexes for each fork
	i = 0;
	while (i < NUM)
	{
		if (pthread_mutex_init(&forks_to_eat[i], NULL) != 0)
		{
			printf("Mutex initialization failed\n");
			perror("Mutex initialization failed\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	// Create threads for each philosopher
	i = 0;
	while (i < NUM)
	{
		philo_id[i] = i;
		if (pthread_create(&philo_threads[i], NULL, philosopher, &philo_id[i]) != 0)
		{
			printf("Mutex creation failed\n");
			perror("Mutex creation failed\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	// Join threads (wait for them to finish)
	i = 0;
	while (i < NUM)
	{
		if (pthread_join(philo_threads[i], NULL) != 0)
		{
			printf("Mutex joining failed\n");
			perror("Mutex joining failed\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}

	// Destroy mutexes
	i = 0;
	while (i < NUM)
	{
		if (pthread_mutex_destroy(&forks_to_eat[i]) != 0)
		{
			printf("Mutex destruction failed\n");
			perror("Mutex destruction failed\n");
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return 0;
}
