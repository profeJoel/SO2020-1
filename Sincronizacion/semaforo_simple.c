//SEMAFORO SIMPLE
/*
H0, H1, H2 -> _______________________________ <- H3, H4, H5
*/
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define CANT_HILOS 5
pthread_t hilo[CANT_HILOS];
int recurso = 0;
int semaforo;

void tomar_ticket(int *s)
{
	while(*s <= 0)
	{
		printf("SEMAFORO EN ROJO!, Hilo esperando...\n");
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
	pthread_t tid;
	tid = pthread_self();
	printf("Semaforo antes: %d\n", semaforo);
	tomar_ticket(&semaforo);
	printf("Semaforo durante: %d\n", semaforo);
	//INICIO SECCION CRITICA
	printf("HILO %ld <%ld>: Estoy iniciando seccion Critica: recurso = %d\n", id, tid, recurso);
	recurso += 1;
	printf("HILO %ld <%ld>: Estoy finalizando seccion Critica: recurso = %d\n", id, tid, recurso);
	//FINAL SECCION CRITICA
	sleep(2);
	terminar_turno(&semaforo);
	printf("Semaforo despues: %d\n", semaforo);
}

int main()
{
	long i;
	int confirmacion;
	semaforo = 3; // si es 1 -> el comportamiento es igual al cerrojo
	for(i=0; i<CANT_HILOS; i++)
	{
		confirmacion = pthread_create(&hilo[i], NULL, &hacer_algo, (void *) i);
		if(confirmacion != 0)
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
	}

	for(i=0; i< CANT_HILOS; i++)
		pthread_join(hilo[i], NULL);

	return 0;
}