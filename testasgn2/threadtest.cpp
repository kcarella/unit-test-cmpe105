#include<pthread.h>
#include<stdio.h>


const int NUM_THREADS = 5;

pthread_mutex_t lock;
int x = 0;

struct thread_info {
	int id;
};

void *start_fn(void *arg)
{
	struct thread_info *ti =(thread_info*) arg;
	printf("Hello from spawned thread! : %d\n", ti->id);
	int self = pthread_self();
	printf("self = %d\n", self);
	pthread_mutex_lock(&lock);
	for(int i = 0; i < 1000; i++) {
		x++;
	}
	printf("x = %d\n", x);
	pthread_mutex_unlock(&lock);
}

int main()
{
	pthread_mutex_init(&lock, NULL);

	pthread_t thread[NUM_THREADS];
	struct thread_info tinfo[NUM_THREADS];

	for(unsigned i = 0; i < NUM_THREADS; i++) {
		tinfo[i].id = i;
		pthread_create(&thread[i], NULL, start_fn, &tinfo[i]);
	}
	// ...
	for(;;);
}