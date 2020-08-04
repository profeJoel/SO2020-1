//Cerrojo simple

#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<unistd.h>

#define BLOQUEADO -1
#define LIBERADO 1

pthread_t hilo[5];
int recurso = 0;
int cerrojo = LIBERADO;

void bloquear_cerrojo(int *c, long id)
{
	do{
		printf("Hilo <%ld>: Esperando...\n", id);
		sleep(0.15);
	}while(*c == BLOQUEADO);
	if(*c == LIBERADO)
	{
		*c = BLOQUEADO;
		printf("CERROJO BLOQUEADO\n");
	}
}

void desbloquear_cerrojo(int *c)
{
	*c = LIBERADO;
	printf("CERROJO LIBERADO\n");
}

void* hacer_algo(void *arg)
{
	long id = (long) arg;

	bloquear_cerrojo(&cerrojo, id);
	//INICIO SECCION CRITICA
	printf("HILO <%ld>: Estoy iniciando seccion Critica: recurso = %d\n", id, recurso);
	recurso += 1;
	printf("HILO <%ld>: Estoy finalizando seccion Critica: recurso = %d\n", id, recurso);
	//FINAL SECCION CRITICA
	sleep(0.1);
	desbloquear_cerrojo(&cerrojo);

}

int main()
{
	long i;
	int confirmacion;
	for(i=0; i<5; i++)
	{
		confirmacion = pthread_create(&hilo[i], NULL, &hacer_algo, (void *) i);
		if(confirmacion != 0)
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
	}

	pthread_join(hilo[0], NULL);
	pthread_join(hilo[1], NULL);
	return 0;
}