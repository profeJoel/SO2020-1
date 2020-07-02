#include<stdio.h>
#include<unistd.h>
#include<signal.h>

int main()
{
	//int kill(pid, signal)
	// > 0 -> se identifica a un proceso
	// == 0 -> se va a lanzar a cualquier proceso del grupo de procesos
	// < -1 -> especificar el ppgid al que se va a lanzar
	// == -1 -> lanzada a cualquier proceso (hacia todos)
	// NUNCA EN LA VIDA lanzar : kill -9 -1

	//Retorno 
	// 0 -> la sinal se envio
	// -1 -> no se envio
	int confirmacion;
	confirmacion = kill(40733, SIGALRM);
	if(confirmacion == 0)
		printf("Exito al enviar signal");
	else
		printf("SIn exito\n");
	return 0;
}