//CERROJO SIMPLE
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define CANT_HILOS 5

pthread_t hilos[CANT_HILOS];
int recurso = 0;
int semaforo;

int tomar_ticket(int *s)
{
	while(*s <= 0)
	{
		printf("Semaforo en Rojo!, Hilo devuelto a la cola\n");
		sleep(1);
	}
	*s -= 1;
}

void terminar_turno(int *s)
{
	*s += 1;
}

void* hacer_algo(void *arg)
{
	long id = (long) arg;
	int ticket;
	pthread_t tid;
	tid = pthread_self();//esperar
	printf("\nSemaforo antes = %d\n", semaforo);
	tomar_ticket(&semaforo);
	printf("\nSemaforo durante = %d\n", semaforo);

	printf("HILO %ld <%ld> : Estoy iniciando seccion Critica con recurso %d\n", id, tid, recurso);

	//INICIO de SECCION CRITICA
	recurso += 1;
	//TERMINAR SECCION CRITICA
	
	printf("HILO %ld <%ld> : Estoy saliendo seccion critica con recurso %d\n", id, tid, recurso);
	sleep(2);
	//señal
	terminar_turno(&semaforo);
	printf("\nSemaforo despues = %d\n", semaforo);
}

int main()
{
	long i;
	int confirmacion;
	semaforo = 2;

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