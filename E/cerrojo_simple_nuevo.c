//CERROJO SIMPLE
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>

#define BLOQUEDO -1
#define LIBERADO  1

pthread_t hilos[5];
int contador = 0;
int cerrojo = LIBERADO;

void bloquear_cerrojo(int *c, long i)
{
	do{
		//printf("HILO <%ld> : ESPERANDO...\n", i);
		printf("\n");
	}while(*c == BLOQUEDO);
	if(*c == LIBERADO)
	{
		printf("Cerrojo Bloqueado\n");
		*c = BLOQUEDO;
	}
}

void desbloquear_cerrojo(int *c)
{
	printf("Cerrojo Desbloqueado\n");
	*c = LIBERADO;
}

void* hacer_algo(void *arg)
{
	long i = (long) arg;

	bloquear_cerrojo(&cerrojo, i);

	printf("HILO <%ld> : Estoy iniciando seccion Critica con contador %d\n", i, contador);

	//INICIO de SECCION CRITICA
	contador += 1;
	//TERMINAR SECCION CRITICA
	
	printf("HILO <%ld> : Estoy saliendo seccion critica con contador %d\n", i, contador);
	sleep(0.1);
	desbloquear_cerrojo(&cerrojo);
	
	return NULL;
}

int main()
{
	long i;
	int confirmacion;
	for(i=0; i<5; i++)
	{
		confirmacion = pthread_create(&hilos[i], NULL, &hacer_algo, (void *) i);
		if(confirmacion != 0)
			printf("ERROR: %d, al lanzar el hilo\n", confirmacion);
	}

	pthread_join(hilos[0], NULL);
	pthread_join(hilos[1], NULL);
	pthread_join(hilos[2], NULL);
	pthread_join(hilos[3], NULL);
	pthread_join(hilos[4], NULL);

	return 0;
}