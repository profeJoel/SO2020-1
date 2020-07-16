#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<signal.h>
#include <sys/wait.h>

#define N 4 // jugadores
#define FICHA_EN_PARTIDA 58
#define FICHA_EN_META 0

//Variables globales
int alguien_gano, ficha[N], t;

typedef struct j{
	pid_t pid;
	int turnos;
	int ranking;
}jugador;

void ignorar(int s)
{
	printf("Proceso <%d> dice: Y a quien le importa? Papa frita!\n", getpid());
}

void manejar_usr2(int s)
{
	printf("Proceso Padre <%d> dice: Deja de Pavonearte!\n", getpid());
	alguien_gano = 1;
}

void volver(int s)
{
	t = -1; //-1 significa que va a terminar el ciclo infinito y volvera al padre
	printf("Escuche la senal\n");
}

void lanzar_dado(int s)
{
	t = 1; // 1 -> significa que lanzara el dado
}

void jactarse(int s)
{
	printf("Proceso <%d> dice: Les gane a todos!!!\n", getpid());
	kill(0, SIGUSR2); //les envia una senal a todos los procesos del grupo de procesos, incluso el mismo y su padre
	signal(SIGUSR2,SIG_IGN);
}

int jugar_ludo() // main del proceso hijo
{
	int dado, turno, elegida, cant_fichas_en_partida = N, fichas_aun_jugando = N;
	for(elegida = 0; elegida<N; elegida++)
		ficha[elegida] = FICHA_EN_PARTIDA; // (58) se dejan todas las fichas en la partida

	turno = 0;
	srand(getpid()); // semilla aleatoria

	signal(SIGTERM, &volver);
	signal(SIGUSR2, &ignorar);
	signal(SIGUSR1, &lanzar_dado);

	while(fichas_aun_jugando > 0)
	{
		t = 0; // al iniciar el turno en 0, esta esperando su turno
		do{
			signal(SIGTERM, &volver);
			signal(SIGUSR2, &ignorar);
			signal(SIGUSR1, &lanzar_dado);
		}while(t == 0);

		printf("Proceso <%d> -> t=%d\n", getpid(), t);

		if(t == 1) // Lanzar el dado, o comenzar el turno
		{
			t = 0;
			turno++;

			do{
				dado = (rand()%6)+1; // lanzo el dado
				printf("Proceso <%d> -> dado =%d\n", getpid(), dado);
				if(cant_fichas_en_partida > 0 && dado == 6) // fichas en la partida
				{
					do{
						elegida = rand() % N;
					}while(ficha[elegida] != FICHA_EN_PARTIDA); // se asegura que la ficha este en la partida
					ficha[elegida]--;
					cant_fichas_en_partida--;
					printf("Proceso <%d> -> ficha %d = %d\n", getpid(), elegida, ficha[elegida]);
				}
				else //las fichas en el tablero
				{
					if(cant_fichas_en_partida<N) // si al menos una ficha esta jugando en el tablero
					{
						do{
							elegida = rand() % N;
						}while(ficha[elegida] == FICHA_EN_PARTIDA);
						if(dado <= ficha[elegida]) // solamente se puede realizar la accion si el valor del dado es menor o igual al valor de la ficha
						{
							ficha[elegida] -= dado;
							printf("Proceso <%d> -> ficha %d = %d\n", getpid(), elegida, ficha[elegida]);
						}
						if(ficha[elegida] == FICHA_EN_META) // llego a la meta
						{
							fichas_aun_jugando--;
							printf("Proceso <%d> -> ficha %d Llego a la meta\n", getpid(), elegida);
						}
					}
				}
			}while(dado == 6 && fichas_aun_jugando > 0);
		}

		if(t == -1)
		{
			printf("Proceso <%d> dice: Me voy!\n", getpid());
			return turno;
		}
	}

	//al momento de terminar la ejecucion del proceso, como ha ganado, se jacta de su victoria
	jactarse();
	return turno;
}

int entregar_turno(pid_t p)
{
	return kill(p, SIGUSR1);
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

pid_t buscar_ultimo(jugador *l)
{
	int i;
	for(i=0; i<N; i++)
		if(l[i].ranking == -1)// valor nulo
			return l[i].pid;
}

int main()
{
	jugador ludopata[N];
	pid_t pidH;
	int i, estado, partida_en_ejecucion=1, posicion, cant_procesos_en_ejecucion=N, confirmacion;

	printf("Proceso Padre <%d> dice: \n\nBIENVENIDOS AL LUDO!!!\n\n", getpid());

	for(i=0; i<N; i++)
	{
		ludopata[i].pid = fork(); //desde aca se crea un proceso nuevo en cada iteracion

		if(ludopata[i].pid == 0)
			exit(jugar_ludo()); // lo realiza el proceso hijo
		else // desde aca lo ejecuta el proceso padre
		{
			ludopata[i].turnos = -1; // -1 significa vacio o nulo
			ludopata[i].ranking = -1;
		}
	}
	sleep(2);

	//sistema de turnos
	while(partida_en_ejecucion)
	{
		//un turno a la vez
		for(i=0; i<N; i++)
		{
			alguien_gano = 0;
			//si el proceso indicado esta en juego o no
			if(ludopata[i].ranking == -1)// si sigue jugando
			{
				confirmacion = entregar_turno(ludopata[i].pid);
				if(confirmacion == 0) // si se envio la senal
				{
					printf(">> Turno entregado a <%d>\n", ludopata[i].pid);
					sleep(0.2);

					signal(SIGTERM, &ignorar);
					signal(SIGUSR2, &manejar_usr2); //-> si se ejecuta, entonces permite esperar un proceso que ha terminado
					if(alguien_gano == 1)
					{
						if(cant_procesos_en_ejecucion > 1)
						{
							alguien_gano = 0;
							pidH = wait(&estado);

							posicion = agregar_datos(ludopata, pidH, WEXITSTATUS(estado), (N-cant_procesos_en_ejecucion+1)); //guardamos la informacion en la estructura
							printf("\nProceso ganador:\n>>Ranking %d <%d> con %d turnos", ludopata[posicion].ranking, ludopata[posicion].pid, ludopata[posicion].turnos);
							//mostrar a un ganador
							cant_procesos_en_ejecucion--;
						}
						//la opcion de ejecutar la senal al ultimo proceso
						if(cant_procesos_en_ejecucion == 1)
						{
							pidH = buscar_ultimo(ludopata);
							confirmacion = kill(pidH, SIGTERM);
							if(confirmacion == 0)
							{
								printf("Se lanzo efectivamente la signal a <%d>\n", pidH);
								sleep(2);
								pidH = waitpid(pidH, &estado, 0);
								posicion = agregar_datos(ludopata, pidH, WEXITSTATUS(estado), (N-cant_procesos_en_ejecucion+1)); //guardamos la informacion en la estructura
								printf("\nProceso ganador:\n>>Ranking %d <%d> con %d turnos", ludopata[posicion].ranking, ludopata[posicion].pid, ludopata[posicion].turnos);
								//mostrar a un ganador
								cant_procesos_en_ejecucion--;
							}
							else
								printf("Error al lanzar signal\n");
						}
					}
					//en caso de terminar de ejecutar
					if(cant_procesos_en_ejecucion == 0)
						partida_en_ejecucion = 0;
				}
				else
					printf("Turno se perdio...\n");
			}
		}
	}

	printf("\n\n***************************************FIN DEL JUEGO******************************");
	for(i=0; i<N; i++)
		if(ludopata[i].ranking == 1)
			printf("\n\nPrimer Lugar: %d -> con %d turnos... El proceso con mas suerte!!!", ludopata[i].pid, ludopata[i].turnos);



	return 0;
}