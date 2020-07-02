//pid_t wait(*estado_proceso) -> valor estado_proceso entre [0,255]
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h> //contiene a wait()

#define NUM_HIJOS 8

int hace_algo()
{
	int r;
	srand(getpid()); // semilla con valor único -> pid del proceso
	r = (rand()%256); // obtendrá valor entre [0,255]
	printf("HIJO: pid = %d, valor calculado %d \n", getpid(), r);
	return r;
}

int main()
{
	pid_t pidH;
	int i, estado;

	// se crearán NUM_HIJOS en el ciclo
	for(i=0; i<NUM_HIJOS; i++) 
	{
		pidH = fork(); // creacíón de procesos hijos

		// se ejecuta solamente en el proceso hijo
		// debito a que el valor que retorna fork() es 0
		if(pidH == 0) 
			// se ejecuta la función y se retorna el valor de estado
			// termina ejecución del proceso hijo con exit() informando estado de termino
			exit(hace_algo());
	}

	// desde acá solamente se ejecuta el proceso padre
	// sus hijos terminaron y estan en estado Zombie
	for(i=0; i<NUM_HIJOS; i++)
	{
		//esta instruccion hace esperar al proceso padre por cualquier hijo que termine su trabajo
		pidH = wait(&estado); // devuelve el valor del pid del hijo zombie
		printf("Proceso PADRE = %d >> Proceso Hijo = %d (estado = %d)\n", getpid(),pidH,WEXITSTATUS(estado));
	}

	// esta parte es para que podamos ver al proceso padre buscar a sus hijos
	while(1)
		sleep(10);

	return 0; //0 - terminó exitosamente
}