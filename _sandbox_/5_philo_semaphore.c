/*
This code implements the solution to the Dining Philosophers problem using semaphores. 

The Dining Philosophers problem is a classic synchronization problem where multiple philosophers 
sit around a table and alternate between thinking and eating. 
To prevent deadlocks and starvation, a philosopher can only eat if both of their 
neighboring chopsticks (resources) are available.

Let's go through the code step by step:

The code includes necessary header files: pthread.h for thread management, and semaphore.h for semaphores.

Constants N, THINKING, HUNGRY, and EATING are defined. 
N represents the total number of philosophers, and the other constants represent the states of a philosopher.
An array `state` is declared to represent the state of each philosopher, and 
an array `philo` is declared to store the IDs of the philosophers.
Two semaphores are declared: mutex to provide mutual exclusion while accessing shared resources, 
and `utensils` array which represents the availability of chopsticks.
The `test` function checks if a philosopher can start eating by checking the states of the philosopher 
and its neighbors. If the conditions are met, 
the philosopher changes its state to EATING and prints the action.
The `take_fork` function is called by a philosopher when it wants to eat. 
It changes its state to HUNGRY, checks if it can start eating, and if not, 
waits for a signal from the test function.
The `put_fork` function is called by a philosopher when it has finished eating. 
It changes its state to THINKING, and signals its neighbors to check if they can start eating.
The `philosopher` function contains the main logic for each philosopher. 
It repeatedly calls take_fork and put_fork in a loop.
In the `main` function, the semaphores are initialized, and threads are created for each philosopher. 
The philosophers start in the THINKING state.
Finally, the main thread waits for all philosopher threads to finish using pthread_join.

This code ensures that the philosophers alternate between thinking and eating, 
and no two neighboring philosophers are eating at the same time, preventing deadlock and starvation.

Note: The code doesn't include error handling, so it assumes that the initialization 
of semaphores and thread creation is successful.
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define	NUM 5
#define THINK 2
#define HUNGRY 1
#define EAT 0
#define LEFT (phnum + 4) % NUM
#define RIGHT (phnum + 1) % NUM

int	state[NUM];
int	philo[NUM] = { 0, 1, 2, 3, 4 };

sem_t	mutex;
sem_t	utensils[NUM];

void	test(int phnum)
{
	if (state[phnum] == HUNGRY && state[LEFT] != EAT && state[RIGHT] != EAT)
	{
		state[phnum] = EAT;
		sleep(2);
		printf("Philosopher [%d] takes fork [%d] and [%d]\n", phnum + 1, LEFT + 1, phnum + 1);
		printf("Philosopher [%d] is EAtING\n", phnum + 1);
		sem_post(&utensils[phnum]);
	}
}

void	take_fork(int phnum)
{
	sem_wait(&mutex);
	state[phnum] = HUNGRY;
	printf("Philosopher [%d] is HUNGRY\n", phnum + 1);
	test(phnum);
	sem_post(&mutex);
	sem_wait(&utensils[phnum]);
	sleep(1);
}

void	put_fork(int phnum)
{
	sem_wait(&mutex);
	state[phnum] = THINK;
	printf("Philosopher [%d] put fork [%d] and [%d] down\n", phnum + 1, LEFT + 1, phnum + 1);
	printf("Philosopher [%d] is THINKING\n", phnum + 1);
	test(LEFT);
	test(RIGHT);
	sem_post(&mutex);
}

void	*philosopher(void *num)
{
	int	*i;

	while (1)
	{
		i = (int *)num;
		sleep(1);
		take_fork(*i);
		sleep(0);
		put_fork(*i);
	}
}

int	main()
{
	int			i;
	pthread_t	thread_id[NUM];

	sem_init(&mutex, 0, 1);

	i = 0;
	while (i < NUM)
	{
		sem_init(&utensils[i], 0, 0);
		i++;
	}
	i = 0;
	while (i < NUM)
	{
		pthread_create(&thread_id[i], NULL, philosopher, &philo[i]);
		printf("Philosopher [%d] is thinking\n", i + 1);
		i++;
	}
	i = 0;
	while (i < NUM)
	{
		pthread_join(thread_id[i], NULL);
		i++;
	}
	return (0);
}
