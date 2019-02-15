//for socket stuff
#include<netdb.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
//for printf
#include<stdio.h>
//for exit
#include<stdlib.h>
#include<unistd.h> 

#include<sys/types.h>
#include<sys/stat.h>
#include <fcntl.h>
#include<errno.h>

//pthread stuff
#include<pthread.h>
#include<semaphore.h>

/*This program shows how different threads run at different speeds.
after multiple runs the outputs will be different. With threads
completing at different times despite order in which orders are called
open is thread safe*/

struct thread_info
{
	int id;
	pthread_mutex_t* mutex_addr;
};


void* thread_func(void* arg)
{
	struct thread_info* ti_ptr = (thread_info*) arg;
	printf("tfunc: thread %d: Hi! I'll wait now..\n", ti_ptr->id);
	pthread_mutex_lock(ti_ptr->mutex_addr);
	printf("tfunc: thread[%d] is in the CR\n", ti_ptr->id);
}

void dispatcher(pthread_mutex_t* addrof_mutex)
{
	//CREATE THREADS
	pthread_t th_array[3];
	struct thread_info tinfo_array[3];
	for(int i = 0; i < 3; i++)
	{
		tinfo_array[i].id = i;
		tinfo_array[i].mutex_addr = addrof_mutex;
		pthread_create(&th_array[i], NULL, thread_func, &tinfo_array[i]);
	}
	//END CREATE THREADS
	sleep(2);
	printf("dispatcher: i will unlock the mutex 1st time\n");
	pthread_mutex_unlock(addrof_mutex);
	sleep(1);
	printf("dispatcher: i will unlock the mutex a 2nd time\n");
	pthread_mutex_unlock(addrof_mutex);

}

int main(int argc, char** argv)
{
	
	//sem_t dipsem;
	//sem_init(&dipsem, 0, 0); //initialize sem
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex, NULL);
	dispatcher(&mutex);
	printf("Main: Main will now loop forever\n");
	for(;;);
	return 0;
}
