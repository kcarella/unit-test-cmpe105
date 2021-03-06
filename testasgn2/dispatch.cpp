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

struct thread_info
{
	int id;
	sem_t* dipsem_addr; 
};


void* thread_func(void* arg)
{
	struct thread_info* ti_ptr = (thread_info*) arg;
	printf("Hi! from thread %d, I'll wait now..\n", ti_ptr->id);
	sem_wait(ti_ptr->dipsem_addr);

	printf("thread %d is finished\n", ti_ptr->id);
}

void dispatcher(sem_t* addrof_dipsem)
{
	//Question: create semaphore here or in main?
	//CREATE THREADS
	pthread_t th_array[3];
	struct thread_info tinfo_array[3];
	for(int i = 0; i < 3; i++)
	{
		tinfo_array[i].id = i;
		tinfo_array[i].dipsem_addr = addrof_dipsem;
		pthread_create(&th_array[i], NULL, thread_func, &tinfo_array[i]);
	}
	for(int i; i < 10000; i++)
	{

	}
	//END CREATE THREADS
	sleep(2);
	printf("I will awaken one thread now\n");
	sem_post(addrof_dipsem);
	sleep(2);
	printf("I will awaken another thread now\n");
	sem_post(addrof_dipsem);

}

int main(int argc, char** argv)
{
	
	sem_t dipsem;
	sem_init(&dipsem, 0, 0); //initialize sem
	dispatcher(&dipsem);

	for(;;);
	return 0;
}
