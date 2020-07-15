#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define N 4 // jugadores
#define FICHA_EN_PARTIDA 58
#define FICHA_EN_META 0

// variables globales de jugadores
int ficha[N],t,alguien_gano=0;

typedef struct j{
	pid_t pid;
	int turnos;
	int ranking;
} jugador;

void ignorar(int s)
{
	printf("Proceso %d dice: Y a quien le importa? papa frita!\n", getpid());
}

void volver(int s)
{
	printf("Te escuche\n");
	t = -1;
}

void lanzar_dado(int s)
{
	//printf("Proceso: %d -> SIGUSR1\n", getpid());
	t = 1;
}

void jactarse()
{
	printf("Proceso %d dice: Les gane a los giles!\n", getpid());
	kill(0,SIGUSR2); // le envía un mensaje a todos -> 0 indica que manda una señal a todos los procesos del grupo de procesos (incluso a él mismo)
	signal(SIGUSR2,SIG_IGN);
}

int jugar_ludo()
{
	int dado, turno, elegida, cant_fichas_en_partida = N, fichas_aun_jugando = N;
	for(elegida=0; elegida<N; elegida++)
		ficha[elegida] = FICHA_EN_PARTIDA; // se dejan todas las fichas en la partida
		
	turno = 0;

	srand(getpid()); //semilla aleatoria

	signal(SIGTERM,&volver);
	if(t==-1)
		return turno;
	signal(SIGUSR2,&ignorar);
	signal(SIGUSR1,&lanzar_dado); // si se gatilla esta funcion, cambia la estado de t.

	while(fichas_aun_jugando > 0)
	{
		t = 0; // iniciar turno en 0, es decir, esperando el turno

		do{
			signal(SIGUSR2,&ignorar);
			signal(SIGUSR1,&lanzar_dado); // si se gatilla esta funcion, cambia la estado de t.
			signal(SIGTERM,&volver);
		}while(t == 0);
		
		printf("PROCESO: %d ->> t = %d\n", getpid(),t);

		if(t==1) // iniciar turno
		{
			t = 0;
			turno++;
			do{
				dado = (rand()%6) + 1; // lanzar dado
				printf("Proceso: %d -> dado: %d\n", getpid(), dado);

				if(cant_fichas_en_partida > 0 && dado == 6) // verifica si es posible usar el turno para sacar fichas de la partida
				{
					//printf("Sale de la zona de partida\n");
					do{
						elegida = rand()%N; // elegir ficha a jugar
						//printf("En ciclo -> ficha: %d [%d] - %d\n",elegida, ficha[elegida], FICHA_EN_PARTIDA);
					}while(ficha[elegida] != FICHA_EN_PARTIDA); // se asegura de elegir una ficha en la partida
					//printf("Ficha elegida: %d\n", elegida);
					ficha[elegida]--;
					cant_fichas_en_partida--;
					printf("Proceso: %d -> ficha: %d -> valor : %d -> fichas en partida %d\n", getpid(), elegida, ficha[elegida],cant_fichas_en_partida);
				}
				else
				{
					if(cant_fichas_en_partida < N)
					{
						do{
							elegida = rand()%N; // elegir ficha a jugar
						}while(ficha[elegida] == FICHA_EN_PARTIDA); // se asegura de elegir una ficha fuera de la partida y no en la meta
						if(dado <= ficha[elegida]) // verifica si se puede realizar el avance dado por el dado
							ficha[elegida] -= dado;
						printf("Proceso: %d -> ficha: %d -> valor : %d\n", getpid(), elegida, ficha[elegida]);
						if(ficha[elegida] == FICHA_EN_META) // verifica si una ficha esta en la meta
						{
							fichas_aun_jugando--;
							printf("Proceso: %d -> ficha: %d llegó a la meta!\n", getpid(), elegida);
						}
					}
				}
			}while(dado == 6 && fichas_aun_jugando > 0); // caso en que el dado es 6 y se puede seguir usando el turno
		}

		if(t == -1){
			printf("ya voy!\n");
			return turno;
		}
	}
	//salir del ciclo significa que ya no quedan fichas para jugar, y enviar un mensaje de jactancia
	jactarse();
	return turno;
}


void manejar_usr2()
{
	printf("\nDeja de pavonearte!\n");
	alguien_gano=1;
}

pid_t buscar_ultimo(jugador *l)
{
	int i;
	for(i=0; i<N; i++)
		if(l[i].ranking == -1)
			return l[i].pid;
}

int agregar_datos(jugador *l, pid_t p, int t, int r)
{
	int x;
	for(x=0; x<N; x++)
		if(l[x].pid == p)
		{
			l[x].turnos = t;
			l[x].ranking = r;
			return x;
		}
}

int entregar_turno(pid_t p)
{
	return kill(p,SIGUSR1);
}

int main(void)
{
	jugador ludopata[N];
	pid_t pidH;
	int i, estado, partida_en_ejecucion=1, posicion, cant_procesos_en_ejecucion=N,confirmacion;

	printf("Proceso Padre %d dice:\n", getpid());
	printf("\n\nBIENVENIDO AL LUDO!!!\n\n");
	for(i=0; i<N; i++)
	{
		ludopata[i].pid = fork();

		if(ludopata[i].pid == 0)
			exit(jugar_ludo());
		else
		{
			ludopata[i].turnos = -1;
			ludopata[i].ranking = -1;
			signal(SIGUSR2,&ignorar);
			signal(SIGTERM,&ignorar);
		}	
	}
	sleep(2);

	signal(SIGUSR2,&ignorar);
	signal(SIGTERM,&ignorar);

	//Sistemas de Turnos de jugadores
	printf("Jugadores que van ganando:\n");
	while(partida_en_ejecucion)
	{
		// un turno a la vez
		for(i=0; i<N; i++)
		{
			alguien_gano = 0;
			if(ludopata[i].ranking == -1)
			{
				confirmacion = entregar_turno(ludopata[i].pid);
				if(confirmacion==0)
				{
					printf("Turno entregado a %d\n", ludopata[i].pid);
					sleep(0.1);

					signal(SIGTERM,&ignorar);
					signal(SIGUSR2,&manejar_usr2); // si se gatilla, entonces permitir esperar un proceso
					if(alguien_gano == 1)
					{

						if(cant_procesos_en_ejecucion > 1)
						{
							alguien_gano = 0;
							printf("VA a esperar...\n");
							pidH = wait(&estado);
							//pidH = waitpid(ludopata[i].pid,&estado,0);
							
							posicion = agregar_datos(ludopata, pidH, WEXITSTATUS(estado),N-cant_procesos_en_ejecucion+1);
							printf("R = %d >> Proceso = %d >> (turnos = %d)\n", ludopata[posicion].ranking, ludopata[posicion].pid,ludopata[posicion].turnos);

							cant_procesos_en_ejecucion--;
						}
						// funciona hasta que el estado no es mayor a 255, luego se repite, verlo de algun modo.
						if(cant_procesos_en_ejecucion == 1)
						{
							pidH = buscar_ultimo(ludopata);
							confirmacion = kill(pidH, SIGTERM);
							if(confirmacion==0)
							{
								printf("Se lanzó efectivamente la senhal: %d\n",pidH);
								sleep(2);
								pidH = waitpid(pidH,&estado,0);
								posicion = agregar_datos(ludopata, pidH, WEXITSTATUS(estado),N-cant_procesos_en_ejecucion+1);
								printf("\nR = %d >> Proceso = %d >> (turnos = %d)\n\n", ludopata[posicion].ranking, ludopata[posicion].pid,ludopata[posicion].turnos);
								cant_procesos_en_ejecucion--;
							}
							else
								printf("Error al lanzar señal\n");
						}
					}

					if(cant_procesos_en_ejecucion == 0)
						partida_en_ejecucion = 0;
				}
				else
				{
					printf("Turno Perdido...\n");
				}
			}
		}
	}
	printf("\n********************FIN DEL JUEGO*****************\n");
	for(i=0; i<N; i++)
		if(ludopata[i].ranking == 1)
			printf("\n\nPrimer Lugar: %d -> con más suerte!\n", ludopata[i].pid);

	return 0;
}