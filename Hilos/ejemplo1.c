// pthread.h
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define N 5

int dato_publico = 0;

void *dice_hola(void *t)
{
	long pthread_id;
	pthread_id = (long) t;
	dato_publico++;
	printf("HILO %ld (%d) dice: HOLA CHICOS de SO -> dato_publico: %d\n", pthread_id,getpid(),dato_publico);
	pthread_exit(NULL);
}

int main()
{
	pthread_t hilos[N];
	int confirmacion;
	long i;
	for(i=0; i<N; i++)
	{
		printf("MAIN: crea el hilo %ld\n", i);
		//pthread_create(pthread_t *hilo, const pthread_attr_t *a, void *(*start_routine) (void*),void *arg) -> *a por defecto : NULL
		confirmacion = pthread_create(&hilos[i],NULL,dice_hola,(void *)i);
		if(confirmacion > 0)
		{
			printf("ERROR al crear: %d\n", confirmacion);
			exit(-1);
		}
	}
	// pthread_join(pthread_t hilo, void **estado) -> estado por defecto : NULL
	for(i=0; i<N; i++)
	{
		confirmacion = pthread_join(hilos[i],NULL);
		if(confirmacion > 0)
		{
			printf("ERROR al recibir: %d\n", confirmacion);
			exit(-1);
		}
	}

	//return 0;
	printf("\nMAIN: El valor de dato_publico: %d\n", dato_publico);
	pthread_exit(NULL);
}