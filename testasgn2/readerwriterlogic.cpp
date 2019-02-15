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
#define TH_NUM 6


/*
This program displays the algorithm having mutually exclusive read/writes. There can either be an infinite number
of readers reading a file, or 1 writer writing to the file. Both of these functions never overlap using the 
following code.
NOTE: attempting to wtite the null character along to a file ends in undefined behavior. DONT count NULL character
WARNING: write and send are different. TEST SEND W/COUNTING NULL AND W/O COUNTING NULL
*/
struct thread_info
{
	int id;
	sem_t* dipsem_addr;
	sem_t* rwsem_addr; //value = 1
	sem_t* mutsem_addr; //value = 1
	char method[20];
	int* rcount_addr;
};


void* thread_func(void* arg)
{
	struct thread_info* ti_ptr = (thread_info*) arg;
	printf("Thread %d: method = %s| I'll wait now..\n", ti_ptr->id, ti_ptr->method);
	sem_wait(ti_ptr->dipsem_addr);
	//write logic here
	if(strcmp(ti_ptr->method, "read") == 0)
	{
		sem_wait(ti_ptr->mutsem_addr); //lock mutex
		*(ti_ptr->rcount_addr)+=1; //incremend readercount
		if(*(ti_ptr->rcount_addr) == 1) //if first reader lock rwsem
		{
			sem_wait(ti_ptr->rwsem_addr);
		}
		sem_post(ti_ptr->mutsem_addr); //unlock mutex

		//READER STUFF, N READERS CAN BE HERE
		int openfd = open("foo.txt", O_RDONLY);
		if(openfd < 0)
		{
			perror("open failed");
			return NULL;
		}
		char readbuff[100];
		pread(openfd, readbuff, 100, 0);
		printf("Thread %d: I read %s", ti_ptr->id, readbuff);
		close(openfd); //<----- may be problematic
		//END READER STUFF

		sem_wait(ti_ptr->mutsem_addr); //lock mutex
		*(ti_ptr->rcount_addr)-=1; //decrement readercount
		if(*(ti_ptr->rcount_addr) == 0)//if last reader, unlock rwsem
		{
			sem_post(ti_ptr->rwsem_addr);
		}
		sem_post(ti_ptr->mutsem_addr);//unlock mutex
	}
	else if(strcmp(ti_ptr->method, "write") == 0)
	{
		sem_wait(ti_ptr->rwsem_addr);
		printf("Thread %d:---------------IN CRITICAL_WRITE\n", ti_ptr->id);

		int openfd = open("foo.txt", O_WRONLY);
		if(openfd < 0)
		{
			perror("open failed");
			return NULL;
		}
		char writebuff[100];
		//strcpy(writebuff, "MODIFY[W]");
		snprintf(writebuff, 10, "MODIFY[%d]", ti_ptr->id);
		int writeval = write(openfd, writebuff, strlen(writebuff));
		if(writeval < 0)
		{
			perror("write failed");
			return NULL;
		}
		close(openfd); //<------ may be problematic
		printf("Thread %d: i wrote %s\n", ti_ptr->id, writebuff);


		sem_post(ti_ptr->rwsem_addr);
	}
	else
	{
		printf("something went wrong\n");
		return NULL;
	}
	//end write logic here
	printf("Thread %d:FINISHED\n", ti_ptr->id);
}

void dispatcher(sem_t* addrof_dipsem, sem_t* addrof_rwsem, sem_t* addrof_mutsem, int* addrof_rcount)
{
	pthread_t th_array[TH_NUM];
	struct thread_info tinfo_array[TH_NUM];
	for(int i = 0; i < TH_NUM; i++)
	{
		tinfo_array[i].id = i;
		tinfo_array[i].dipsem_addr = addrof_dipsem;
		tinfo_array[i].rwsem_addr = addrof_rwsem;
		tinfo_array[i].mutsem_addr = addrof_mutsem;
		tinfo_array[i].rcount_addr = addrof_rcount; 
		if((i % 2) == 0)
		{
			strcpy(tinfo_array[i].method, "read");
		}
		else
		{
			strcpy(tinfo_array[i].method, "write");
		}
		pthread_create(&th_array[i], NULL, thread_func, &tinfo_array[i]);
	}
	sleep(1);
	for(int k = 0; k<TH_NUM;k++)
	{
		sem_post(addrof_dipsem);
	}
	sleep(1);
	printf("dispatch: i will loop forever now\n");
	for(;;);
}

int main(int argc, char** argv)
{
	
	sem_t dipsem;
	sem_t rwsem;
	sem_t mutsem;
	sem_init(&dipsem, 0, 0); 
	sem_init(&rwsem, 0, 1);
	sem_init(&mutsem, 0, 1);
	int rcount = 0;
	dispatcher(&dipsem, &rwsem, &mutsem, &rcount);
	return 0;
}
