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

int main(int argc, char** argv)
{
	int threadnum;
	int blocknum;
	char* hadress;
	int port;
	parseCmdLine(&threadnum, &blocknum, &hadress, &port, argc, argv);
	printf("threadnum = %d\n", threadnum);
	printf("blocknum = %d\n", blocknum);
	printf("hadress = %s\n", hadress);
	printf("port = %d\n", port);
}