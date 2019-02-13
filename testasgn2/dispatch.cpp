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

void parseCmdLine(int* threadnum, int* blocknum,
char** hadress, int* port, int argc, char** argv);

struct thread_info
{
	int id;
	sem_t* dipsem_addr;
};

//global variables
//sem_t dipsem;

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
	/*
	//command line parsing
	int wthreads;
	int blocks;
	int port;
	char* host;
	parseCmdLine(&wthreads, &blocks, &host, &port, argc, argv);
	printf("wthreads = %d\n", wthreads);
	printf("blocks = %d\n", blocks);
	printf("port = %d\n", port);
	printf("host = %s\n", host);
	printf("----------------------------\n");
	//end of command line parsing

	//socket code
	//create sockaddr_in
	struct hostent* hent = gethostbyname(host);
	if(hent == NULL)
	{
		herror("gethostbyname failed");
		exit(1);
	}
	struct sockaddr_in addr;
	memcpy(&addr.sin_addr.s_addr, hent->h_addr, hent->h_length);
	addr.sin_port = htons(port);
	addr.sin_family = AF_INET;
	//create server socket
	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
	{
		perror("socket() failed");
		exit(1);
	}
	//server setup
	int enable = 1;
	if((setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable))) != 0)
	{
		perror("sockopt() failure");
		exit(1);
	}
	if((bind(sockfd, (struct sockaddr *)&addr, sizeof(addr))) != 0)
	{
		perror("bind() failure");
		exit(1);
	}
	//end of socket code
	*/
	sem_t dipsem;
	sem_init(&dipsem, 0, 0); //initialize sem
	dispatcher(&dipsem);

	for(;;);
}

void parseCmdLine(int* threadnum, int* blocknum,
char** hadress, int* port, int argc, char** argv)
{
	//set default values
	*threadnum = 4;
	*blocknum = 40;
	*port = 80;
	char opt;
	while((opt = getopt(argc, argv, ":N:c:")) != -1)
	{
		switch(opt)
		{
			case'N':
			*threadnum = atoi(optarg);
			if(*threadnum <= 0)
			{
				fprintf(stderr, "Usage: invalid thread number\n");
				exit(1);
			}
			break;

			case 'c':
			*blocknum = atoi(optarg);
			if(*blocknum <= 0)
			{
				fprintf(stderr, "Usage: invalid block number\n");
				exit(1);			
			}
			break;

			case ':':
			fprintf(stderr, "Usage: ./server -N [thread_num] -c [block_num] hostadress:port\n");
			exit(1);
			break; 

			case'?':
			fprintf(stderr, "Usage: ./server -N [thread_num] -c [block_num] hostadress:port\n");
			exit(1);
			break;
		}
	}
	if(argv[optind] != NULL)
	{
		char* taddress = strtok(argv[optind], ":");
		if(taddress != NULL)
		{
			*hadress = taddress;
			char* tport = strtok(NULL, "");
			if(tport != NULL)
			{
				*port = atoi(tport);
			}
			
			return; 
		}
	}
	fprintf(stderr, "Usage: ./server -N [thread_num] -c [block_num] hostadress:port\n");
	return;
}	