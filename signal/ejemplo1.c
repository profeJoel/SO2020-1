#include<stdio.h>
#include<unistd.h>
#include<signal.h>

void mi_accion(int s)
{
	printf("He recibido una signal: %d\n", s);
}

int main()
{
	//signal(signal, accion(funcion,signal));
	//signal(SIGALRM,SIG_IGN);
	signal(SIGALRM,&mi_accion);
	while(1)
	{
		printf(">> esto esta en ejecucion\n");
		sleep(2);
	}
	return 0;
}