//Cerrojo POSIX
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

pthread_t hilo[5];
int recurso = 0;
pthread_mutex_t cerrojo;

void* hacer_algo(void *arg)
{
	long id = (long) arg;

	pthread_mutex_lock(&cerrojo);
	//INICIO SECCION CRITICA
	printf("HILO <%ld>: Estoy iniciando seccion Critica: recurso = %d\n", id, recurso);
	recurso += 1;
	printf("HILO <%ld>: Estoy finalizando seccion Critica: recurso = %d\n", id, recurso);
	//FINAL SECCION CRITICA
	sleep(0.1);
	pthread_mutex_unlock(&cerrojo);
}

int main()
{
	long i;
	int confirmacion;
	if(pthread_mutex_init(&cerrojo, NULL) != 0)
		printf("CERROJO INICIADO...\n");
	for(i=0; i<5; i++)
	{
		confirmacion = pthread_create(&hilo[i], NULL, &hacer_algo, (void *) i);
		if(confirmacion != 0)
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
	}

	pthread_join(hilo[0], NULL);
	pthread_join(hilo[1], NULL);
	pthread_join(hilo[2], NULL);
	pthread_join(hilo[3], NULL);
	pthread_join(hilo[4], NULL);
	pthread_mutex_destroy(&cerrojo);
	return 0;
}