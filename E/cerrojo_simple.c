#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

pthread_t tid[2];
int contador;
int cerrojo = 1;

int bloquear_cerrojo(int *c)
{
	if(*c == 0)
	{
		//printf("Cerrojo bloqueado!!!\n");
		return -1;
	}
	*c = 0;
	return 0;
}

void desbloquear_cerrojo(int *c)
{
	*c = 1;
	printf("Cerrojo Desbloqueado!!!\n");
}

void* HacerAlgo(void *arg)
{
	int estado;
	do{
		estado = bloquear_cerrojo(&cerrojo);
	}while(estado == -1);

	unsigned long i = 0;
	contador += 1;
	printf("\nHilo %d iniciado\n", contador);

	for(i=0; i<(0xFFFFFFFF); i++);

	printf("\nHilo %d terminado\n", contador);
	desbloquear_cerrojo(&cerrojo);
	
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