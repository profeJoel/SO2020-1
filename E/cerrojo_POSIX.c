#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int contador;
pthread_mutex_t lock;

void* HacerAlgo(void *arg)
{
	pthread_mutex_lock(&lock);

	unsigned long i = 0;
	contador += 1;
	printf("\nHilo %d iniciado\n", contador);

	for(i=0; i<(0xFFFFFFFF); i++);

	printf("\nHilo %d terminado\n", contador);
	pthread_mutex_unlock(&lock);
	
	return NULL;
}


int main(void)
{
	int i = 0;
	int err;

	if(pthread_mutex_init(&lock, NULL) != 0)
	{
		printf("\nCerrojo\n");
	}

	while( i < 2 )
	{
		err = pthread_create(&(tid[i]), NULL, &HacerAlgo, NULL);
		if(err != 0)
			printf("No se puede crear el Hilo : [%s]\n", strerror(err));
		i++;
	}

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);
	pthread_mutex_destroy(&lock);

	return 0;
}