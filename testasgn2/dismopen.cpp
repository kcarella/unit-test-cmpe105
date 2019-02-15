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
	sem_t* dipsem_addr;
	int fd = -6868; 
};


void* thread_func(void* arg)
{
	struct thread_info* ti_ptr = (thread_info*) arg;
	printf("thread %d: Hi! I'll wait now..\n", ti_ptr->id);
	sem_wait(ti_ptr->dipsem_addr);
	ti_ptr->fd = open("foo.txt", O_RDONLY);
	char readbuf[1000];
	int readval = read(ti_ptr->fd, readbuf, 999);
	if(readval < 0)
	{
		printf("thread %d: read failed\n", ti_ptr->id);
		return NULL;
	}
	printf("thread %d: I read| %s\n", ti_ptr->id, readbuf);
	sleep(2);
	printf("thread %d: I have fd %d\n", ti_ptr->id, ti_ptr->fd);
	printf("thread %d: I'm DONE\n", ti_ptr->id);

}

void dispatcher(sem_t* addrof_dipsem)
{
	//CREATE THREADS
	pthread_t th_array[3];
	struct thread_info tinfo_array[3];
	for(int i = 0; i < 3; i++)
	{
		tinfo_array[i].id = i;
		tinfo_array[i].dipsem_addr = addrof_dipsem;
		pthread_create(&th_array[i], NULL, thread_func, &tinfo_array[i]);
	}
	//END CREATE THREADS
	sleep(2);
	printf("dispatcher:I will awaken one thread now\n");
	sem_post(addrof_dipsem);
	printf("dispatcher: I will awaken another thread now\n");
	sem_post(addrof_dipsem);
	printf("dispatcher: and the final one\n");
	sem_post(addrof_dipsem);
	sleep(3);
	printf("dispatcher: end of dispatcher\n");
}

int main(int argc, char** argv)
{
	
	sem_t dipsem;
	sem_init(&dipsem, 0, 0); //initialize sem
	dispatcher(&dipsem);
	printf("Main: Main will now loop forever\n");
	for(;;);
	return 0;
}
