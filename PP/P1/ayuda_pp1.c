#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<signal.h>

#define N 50 // cantidad de mercenarios

// variables globales de mercenarios
int vds,ptj,ext;

typedef struct j{
	pid_t pid;
	int puntaje;
	int ranking;
} jugador;

int acertar_disparo()
{
	return // (incorpore codigo) enviar signal USR1 a todos 
}

int devolver_vida()
{
	return // (incorpore codigo) enviar signal USR2 a todos 
}

void dormir()
{
	sleep(2);
}

void manejar_signal_usr1(int s)
{
	int o;
	o = rand() % 2;
	if(o == 1)
		// (incorpore codigo) recibe impacto
	else
		// (incorpore codigo) resiste impacto
}

void manejar_signal_usr2(int s)
{
	if(vds < 3)
		vds++;
}

void manejar_signal_term(int s)
{
	ext = 0;
}


int acciones_mercenario()
{
	srand(getpid());
	int op, con;
	ptj = 0; vds = 3; ext = 1;

	while(vds)
	{
		// (incorpore codigo) manejar signal TERM a todos 
		// (incorpore codigo) manejar signal USR1 a todos 
		// (incorpore codigo) manejar signal USR2 a todos 
		
		if(ext == 0)
			return ptj;

		op = (rand()%3);

		switch(op)
		{
			case 0:
				con = // (incorpore codigo) acertar disparo
				ptj += 5;
				break;
			case 1:
				con = // (incorpore codigo) devolver vida
				break;
			case 2:
				// (incorpore codigo) dormir
				break;
		}
	}
	return ptj;
}

int agregar_datos(jugador *m, pid_t p_H, int p, int r)
{
	int i;
	for(i=0; i<N; i++)
		if(m[i].pid == p_H)
		{
			m[i].puntaje = p;
			m[i].ranking = r;
			return i;
		}
}

pid_t buscar_ultimo(jugador *m)
{
	int i;
	for(i=0; i<N; i++)
		if(m[i].puntaje == -1)
			return m[i].pid;
}

void ignorar(int s)
{
	//printf("NO TOMAR EN CUENTA, SOY TU PADRE\n");
	s=0;
}

int main(void)
{
	jugador mercenario[N];
	pid_t pidH;
	int i, estado, partida_en_ejecucion=1, cant_procesos_en_ejecucion=N, posicion,confirmacion;

	printf("BIENVENIDO A OPEN FIRE!!!\n");
	for(i=0; i<N; i++)
	{
		mercenario[i].pid = // (incorpore codigo) crear nuevo proceso hijo

		if(mercenario[i].pid == 0)
			// (incorpore codigo) exija el termino de las acciones del mercenario
		else
		{
			mercenario[i].puntaje = -1;
			mercenario[i].ranking = -1;

			// (incorpore codigo) ignorar signal TERM
			// (incorpore codigo) ignorar signal USR1 
			// (incorpore codigo) ignorar signal USR2 
		}	
	}

	// (incorpore codigo) ignorar signal TERM
	// (incorpore codigo) ignorar signal USR1 
	// (incorpore codigo) ignorar signal USR2 

	printf("Mercenarios caídos\n");
	while(partida_en_ejecucion)
	{
		// (incorpore codigo) ignorar signal TERM
		// (incorpore codigo) ignorar signal USR1
		// (incorpore codigo) ignorar signal USR2

		// funciona hasta que el estado no es mayor a 255, luego se repite, verlo de algun modo.
		if(cant_procesos_en_ejecucion == 1)
		{
			confirmacion = // (incorpore codigo) enviar signal TERM al ultimo hijo 
			if(confirmacion==0)
				printf("Se lanzó efectivamente la senhal\n");
			else
				printf("Error al lanzar señal\n");
			pidH = // (incorpore codigo) espere al ultimo hijo 
		}
		else
			pidH = // (incorpore codigo) espere a cualquier hijo 
		
		posicion = agregar_datos(mercenario, pidH, WEXITSTATUS(estado),cant_procesos_en_ejecucion);
		printf("R = %d >> Proceso = %d >> (puntaje = %d)\n", mercenario[posicion].ranking, mercenario[posicion].pid, mercenario[posicion].puntaje);

		cant_procesos_en_ejecucion--;

		if(cant_procesos_en_ejecucion == 0)
			partida_en_ejecucion = 0;
	}

	printf("\n\nPrimer Lugar: %d -> Se los pitio a todos!\n", mercenario[posicion].pid);

	/*printf("\nTabla de Procesos:\n");
	for(i=0; i<N; i++)
	{
		printf("R = %d >> Proceso = %d >> (puntaje = %d)\n", mercenario[i].ranking, mercenario[i].pid, mercenario[i].puntaje);
	}*/

	return 0;
}
