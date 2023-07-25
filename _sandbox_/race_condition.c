#include <pthread.h>
#include <stdio.h>

/*
** This code illustrates a RACE CONDITION
** A race condition occurs when two or more threads can access shared data and they try to change it 
** at the same time. Because the thread scheduling algorithm can swap between threads at any time, 
** you don't know the order in which the threads will attempt to access the shared data. 
** Therefore, the result of the change in data is dependent on the thread scheduling algorithm, 
** i.e. both threads are "racing" to access/change the data. 
** 
** ..race condition can only occur on the multicore processor, it cannot occure on singlecore processor.


int				count = 0;
pthread_mutex_t	mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct s_args
{
	int	id;
}	t_args;

void  *routine(void *thread_args)
{
	t_args	*args;
	int i;

	args = (t_args *)thread_args;
	i = 0;
	while (i < 100)
	{
		count++;
		printf("\tt_id: [%d], count: [%d]\n", args->id, count);
		i++;
	}
	return (NULL);
}

int main()
{
	pthread_t	tid1;
	pthread_t	tid2;
	t_args		args_1;
	t_args		args_2;

	args_1.id = 1;
	args_2.id = 2;

	pthread_create(&tid1, NULL, &routine, (void *)&args_1);
	pthread_create(&tid2, NULL, &routine, (void *)&args_2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("cont: %d\n", count);
}
*/

/*
The following is the modified version with the use of pthread_mutex_t which alow
to lock the usage of thread at a specific time. This will prevent the race condition.

Mutex is a sort of lock using with the variable.

It is necessary to destroy mutex right after its usage !
*/

int				count = 0;
pthread_mutex_t	mutex_lock;

typedef struct s_args
{
	int	id;
}	t_args;

void  *routine(void *thread_args)
{
	t_args	*args;
	int i;

	args = (t_args *)thread_args;
	i = 0;
	while (i < 100)
	{
		pthread_mutex_lock(&mutex_lock);
		count++;
		printf("\tt_id: [%d], count: [%d]\n", args->id, count);
		pthread_mutex_unlock(&mutex_lock);
		i++;
	}
	return (NULL);
}

int main()
{
	pthread_t	tid1;
	pthread_t	tid2;
	t_args		args_1;
	t_args		args_2;

	args_1.id = 1;
	args_2.id = 2;

	pthread_mutex_init(&mutex_lock, NULL);
	pthread_create(&tid1, NULL, &routine, (void *)&args_1);
	pthread_create(&tid2, NULL, &routine, (void *)&args_2);

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_mutex_destroy(&mutex_lock);
	printf("cont: %d\n", count);
}
