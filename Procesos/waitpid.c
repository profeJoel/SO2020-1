//waitpid(pid, *status, options) // options = 0
//wait(*estado_proceso) - [0,255]
// 0 - EXIT_SUCCESS
// 1 - EXIT_FAILURE

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define NUM_HIJOS 8

int hace_algo()
{
	int r;
	srand(getpid());
	r = (rand()%256);
	printf("HIJO: pid = %d, valor calculado %d \n", getpid(), r);
	return r;
}

int main()
{
	pid_t pidH, pidHIJOS[NUM_HIJOS];
	int i, estado;

	for(i=0; i<NUM_HIJOS; i++)
	{
		//pidH = fork();
		pidHIJOS[i] = fork();

		if(pidHIJOS[i] == 0)
			//con este comando terminan su ejecución 
			//NO vuelven al ciclo
			exit(hace_algo());

		//Solo el padre vuelve al ciclo
	}

	// Especificacion argumento PID
	// OBSERVACION: pgid es el identificador de un grupo de procesos
	// < -1 (o -pgid): la funcion espera a cualquier proceso que termine del grupo pgid indicado
	// = -1 (o WAIT_ANY): la funcion espera a cualquier proceso hijo (igual a wait())
	// =  0 (o WAIT_MYPGRP): la funcion espera a cualquier proceso en el mismo grupo de procesos del proceso que lo ejecuta
	// >  0 (o pid): la funcion espera al proceso indicado

	// Especificacion argumento option
	// option es un entero que representa 0 o más flags especificados en wait.h
	// 0: indica que no hay un comportamiento especial
	// bandera WNOHANG  : indica que el proceso padre no deberia esperar y la funcion retorna 0
	// bandera WUNTRACED: indica que se retorna el estado del proceso parado y de los procesos que han terminado

	pidH = waitpid(pidHIJOS[0],&estado, 0);
	printf("Proceso PADRE = %d >> Proceso Hijo = %d (estado = %d)\n", getpid(),pidH,WEXITSTATUS(estado));

	//Recuperar estados del proceso con macros de wait.h
	// WEXITSTATUS(*estado) : evalua el estado de un proceso terminado con exit()
	// WIFEXITED(*estado) : evalua el estado de un proceso terminado normalmente o con exit()
	// WIFSIGNALED(*estado) : evalua el estado de un proceso terminado por una señal
	// WIFSTOPPED(*estado) : evalua el estado de un proceso que esta detenido o en espera
	// WSTOPSIG(*estado) : evalua el numero de señales que pararon el proceso hijo
	// WTERMSIG(*estado) : evalua el numero de señales que terminaron el proceso hijo

	while(1)
		sleep(10);

	return 0; //0 - termino exitosamente
}