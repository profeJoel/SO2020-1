#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int contador=0;

void* HacerAlgo(void *arg)
{
	unsigned long i = 0;
	contador += 1;
	printf("\nHilo %d iniciado\n", contador);

	for(i=0; i<(0xFFFFFFFF); i++);

	printf("\nHilo %d terminado\n", contador);
	
	return NULL;
}


int main(void)
{
	int i = 0;
	int err;

	while( i < 2 )
	{
		err = pthread_create(&(tid[i]), NULL, &HacerAlgo, NULL);
		if(err != 0)
			printf("No se puede crear el Hilo : [%s]\n", strerror(err));
		i++;
	}

	pthread_join(tid[0], NULL);
	pthread_join(tid[1], NULL);

	return 0;
}