#include <stdio.h>
#include <unistd.h>

int COVID = 19;

int main(void)
{
	pid_t pidHijo;
	
	printf(">> PID Padre = %d >> comienza\n", getpid());
	pidHijo = fork();

	printf(">> PID Padre = %d >> PID Hijo = %d >> corriendo\n", getpid(), pidHijo);

	if(pidHijo > 0)// Ejecuta el padre
	{
		COVID = 20;
	}
	else if(pidHijo == 0)// ejecuta el Hijo
	{
		COVID = 40;
	}
	else
	{
		printf("\nHubo un Error\n");
	}

	while(1)
	{
		sleep(2);
		printf("El valor COVID=%d por PID=%d\n", COVID, getpid());
	}
	return 0;
}