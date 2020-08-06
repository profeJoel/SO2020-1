#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define CANT_HILOS 5

pthread_t hilos[CANT_HILOS];

sem_t semaforito;
int recurso = 0;

void* hacer_algo(void* arg)
{
	long id = (long) arg;
	int ticket;
	pthread_t tid;
	tid = pthread_self();//esperar
	sem_wait(&semaforito);

	printf("HILO %ld <%ld> : Estoy iniciando seccion Critica con recurso %d\n", id, tid, recurso);

	//INICIO de SECCION CRITICA
	recurso += 1;
	//TERMINAR SECCION CRITICA
	printf("HILO %ld <%ld> : Estoy saliendo seccion critica con recurso %d\n", id, tid, recurso);
	sleep(2);
	//señal
	sem_post(&semaforito);
}

int main()
{
	long i;
	int confirmacion;
	sem_init(&semaforito, 0, 2);

	for(i=0; i<CANT_HILOS; i++)
	{
		confirmacion = pthread_create(&hilos[i], NULL, &hacer_algo, (void *) i);
		if(confirmacion != 0)
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
	}

	for(i=0; i<CANT_HILOS; i++)
		pthread_join(hilos[i], NULL);

	return 0;
}