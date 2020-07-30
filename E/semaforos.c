#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t semaforito;
int contador = 0;

void* hilo(void* arg)
{
	pthread_t tid;
	tid = pthread_self();
	//esperar
	sem_wait(&semaforito);
	printf("\nHa Entrado...\n");

	//sección Crítica
	contador += 1;
	printf("Inicio de hilo: %d - %u\n", contador, (unsigned int )tid);
	
	sleep(4); //toda la rutina debiera estar en esta sección
	
	printf("Fin de hilo: %d - %u\n", contador, (unsigned int )tid);
	//señal
	printf("\nHa Salido...\n");
	sem_post(&semaforito);
}

int main()
{
	sem_init(&semaforito, 0, 1);
	pthread_t t1, t2;
	pthread_create(&t1, NULL, hilo, NULL);
	sleep(2);
	pthread_create(&t2, NULL, hilo, NULL);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	sem_destroy(&semaforito);
	return 0;
}