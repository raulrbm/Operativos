//Raul R Bander
//Carnet: 12-11285
//Fabiana J Acosta
//Carnet: 03- ---------> NAF: Pon aqui el resto de tu carnet <----------------
//Proyecto 1 Sistemas de Operacion

//Estructuras a utilizar
struct objetivo{
	
	int coordX;
	int coordY;
	int val;
	char tipo;
}

struct bomba{

	int coordX;
	int coordY;
	int radio;
	int pot;
}

//------------------------------------------------------------(1)-----------------------------------------------------------------------
//Inicializamos las variables donde guardaremos los datos de la entrada
int p = 0;//Indica si el programa se ejecutara por procesos
int h = 0;//Indica si el proceso se ejecuta por hilos
int n = 0;//Guarda el numero de procesos ayudantes que debe crear el programa
char* fl;//String con el nombre del archivo

//Recivimos los datos de la entrada

/* NAF: Palma recomienda usar getopt o arg_parse, no entendi muy bien como se usa
asiq hice un codigo sin usarlas, si manejas alguna o sientes q lo puedes hacer mejor 
usando una brutal, te dejo los links q dio palma y uno de stackoverflow donde medio
explican la vaina:
http://stackoverflow.com/questions/9642732/parsing-command-line-arguments
https://www.gnu.org/software/libc/manual/html_node/Argp.html#Argp*/

int i = 1;//Contador para recorrer el arreglo con las entradas
char* e;//Variable que almacena la entrada actual
e = argv[0];//Guardamos en e la primera entrada
while (e != void){// ---------> NAF: No se si cuando revisas una entrada vacia da void o que <----------------
	if (e == "-h"){// ---------> NAF: No se si puedes comparar asi los strings en c, creo q no <--------------
		h = 1;
	}
	else if (e == "-p"){
		p = 1;
	}
	else if (e == "-n"){//Si e = -n tomamos la siguiente entrada como numero de procesos
		i = i + 1;
		n = (int)argv[i]// ---------> NAF: No se si este casteo es de c o c++ en caso de q este mal debe ser atoi(argv[i])<--------------
	}
	else{
		fl = e;
	}
	i = i + 1;
}
if (p == 1 && h == 1){
	printf("Error: No se puede ingresar las opciones -p y -h simultaneamente")
	exit();//---------> NAF: No estoy claro de si asi se sale del programa <--------------
}
if (p == 0 && h == 0){
	p = 1;
}

//Abrimos el archivo
FILE *fp;
fp = fopen ( argv[1] , "r" );
if ( fp == NULL){
	printf ("Error al abrir el archivo");
	exit();
}

int size; //Tama;o del campo de batalla
int nobj; //Numero de objetivos
int nbomb;//Numero de bombas
char linea[10];//Linea leida del archivo

//Leemos el archivo
linea = fgets(linea, 10, f);// ---------> NAF: Si esto explota trata poniendo el 10 en 100 <--------------
size = (int)linea;//Guardamos el tam;o del campo de batalla
linea = fgets(linea, 10, f);
nobj = (int)linea;//Guardamos el numero de objetivos
struct objetivo objs[nobj];//Arreglo con los objetivos
struct objetivo obj;//Objetivo auxiliar
i = 0;//Reciclamos el contador del ciclo anterior
int ocs = 0; //Numero de objetivos civiles
int oms = 0;//Numero de objetivos militares
while (i < nobj){
	linea = fgets(linea, 10, f);
	e = strtok(linea," ");
	obj.coordX = (int)e;
	e = strtok(linea," ");
	obj.coordY = (int)e;
	e = strtok(linea," ");
	obj.val = (int)e;
	if (obj.val >= 0){//En caso de que el objetivo sea civil
		obj.tipo = "c";
		ocs = ocs + 1;
	}
	else{//En caso de que el objetivo sea militar
		obj.tipo = "m";
		oms = oms + 1;
	}
	objs[i] = obj;
	i = i + 1;
}
linea = fgets(linea, 10, f);
nbomb = (int)linea;//Guardamos el numero de bombas
struct bomba bombs[nbomb];//Arreglo con las bombas
struct bomba bomb;//Bomba auxiliar
i = 0;//Reciclamos el contador del ciclo anterior
while (i < nbomb){
	linea = fgets(linea, 10, f);
	e = strtok(linea," ");
	bomb.coordX = (int)e;
	e = strtok(linea," ");
	bomb.coordY = (int)e;
	e = strtok(linea," ");
	bomb.radio = (int)e;
	e = strtok(linea," ");
	bomb.pot = (int)e;
	bombs[i] = bomb;
	i = i + 1;
}
fclose (fp); //Cerramos el archivo

//---------------------------------------------------------------(2)-------------------------------------------------------------------
int cdb[size][size];//campo de batalla
int j;//Contador para el ciclo interno
i = 1;//Reciclamos el contador del ciclo anterior
obj = objs[0];//Reciclamos el objetivo auxiliar
while (i < size){//Inicializamos el campo de batalla en -1
	j = 0;
	while (j < size){
		cdb[i][j] = -1;
		j = j + 1;
	}
	i = i + 1;
}
while (i < nobj){//Guardamos la ubicacion de cada objetivo
	cdb[obj.coordX][obj.coordY] = i;
	i = i + 1;
}


//--------------------------------------------------------------(3)-------------------------------------------------------------------
int rep;//Numero de bombas que le toca a cada proceso/hilo
int repp = nbomb;//Numero de bombas que le toca al proceso/hilo principal
if (n != 0){
	rep = nbomb / (n+1);
	repp = rep;
	if (nbomb % (n+1) != 0){//Si la divicion no es exacta el resto se le asignara al padre
		repp = rep + (nbomb % (n+1));
	}
}

//-----------> NAF: Aqui va el if q dice si trabajamos con hilos o procesos, como este codigo solo lo hare para procesos no lo puse <------------

//-------------------------------------------------------------(4)---------------------------------------------------------------------
/* NAF: Esta es una de las partes tricky del proyecto, creo que es como lo voy a hacer pero no estoy claro, tengo que consultar con
palma si la idea esta bien pero si hay un error es posible que sea en esta parte.*/

//Creamos el primer proceso hijo
int ret;//Guardamos el valor de retorno del fork para identificar quien es el hijo y quien es el padre
int pids[n];//Arreglo que contiene el pid de los procesos hijos
i = 0;//Reciclamos el contador del ciclo anterior
while (i < n){//Inicializamos todos los elementos del arreglo en -1
	pids[i] = -1;
}
ret = fork();
pids[0] = ret;//Guardamos el pid del proceso hijo(Proceso padre) //Guardamos 0 en nuestra posicion de pids(Proceso hijo)
i = 1;//Reciclamos el contador del ciclo anterior
while (i < n){//Creamos los n - 1 procesos restantes
	if (ret != 0){//Si somos el proceso principal
		ret = fork();
		pids[i] = ret;//Guardamos el pid del proceso hijo(Proceso padre) //Guardamos 0 en nuestra posicion de pids(Proceso hijo)
		i = i + 1;
	}
	if (ret == 0){//Si no somos el proceso principal ---> NAF: Si esto genera algun problema cambiarlo por un else <---
		break;
	}
}

//-----------------------------------------------------------(5)-------------------------------------------------------------------------

struct bomba myBombs[repp];//Arreglo que guarda las bombas que le tocaron a los procesos
i = 0;//Reciclamos la variable del ciclo anterior
j = 0;//Raciclamos el contador del ciclo anterior
if (ret == 0){//Para los procesos hijos
	while (i < n){
		if (pids[i] == 0){
			while (j < rep){
				myBombs[j] = bombs[repp+j+(i*rep)]; //------> NAF: Tengo dudas sobre los efectos de borde en este caso <------
				j = j + 1;
			}
		}
		i = i + 1;
	}
}
else{//Para el proceso padre
	while (j < repp){
		myBombs[j] = bombs[j];
		j = j + 1;
	}
}

//------------------------------------------------------------(6)------------------------------------------------------------------------
/* NAF: Esta forma de resolverlo es terriblemente ineficiente pero no se me ocurrio una mejor, 
pense en una estructura dinamica pero las cosas con apuntadores darian muchos problemas a la hara
de retornarselas al proceso padre, en la semana pedire ayuda y si se me ocurre algo mejor te aviso
y lo cambio*/

i = 0;//Reciclamos el contador del ciclo anterior
int k;//Contador para el ciclo mas interno
int res[size][size];//Campo de batalla con el resultado de los bombardeos
while (i < size){
	j = 0;//Reciclamos el contador del ciclo anterior
	while (j < size){
		if (res[i][j] != -1){
			res[i][j] = 0
		}
		j = j + 1;
	}
	i= i + 1;
}
if (ret == 0){
	i = 0;//Reciclamos el contador del ciclo anterior
	while (i < rep){
		bomb = mybombs[i];//Reciclamos la bomba auxiliar
		j = bomb.coordX - bomb.radio;//Reciclamos el contador del ciclo anterior
		while (j < bomb.coordX + bomb.radio){//Recorremos hasta el final del tablero
			k = bomb.coordY - bomb.radio//
			while (k < bomb.coordY + bomb.radio){
				if (cdb[j][k] != -1){
					res[j][k] = res[j][k] + bomb.pot;//Guardamos el daño que ha sufrido el objetivo hasta ahora
				}
				k = k + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
}
if (ret == 0){
	i = 0;//Reciclamos el contador del ciclo anterior
	while (i < rep){
		bomb = mybombs[i];//Reciclamos la bomba auxiliar
		j = bomb.coordX - bomb.radio;//Reciclamos el contador del ciclo anterior
		while (j < bomb.coordX + bomb.radio){//Recorremos hasta el final del tablero
			k = bomb.coordY - bomb.radio//
			while (k < bomb.coordY + bomb.radio){
				if (cdb[j][k] != -1){
					res[j][k] = res[j][k] + bomb.pot;//Guardamos el daño que ha sufrido el objetivo hasta ahora
				}
				k = k + 1;
			}
			j = j + 1;
		}
		i = i + 1;
	}
}

//-----------------------------------------------------------(7)(8)-----------------------------------------------------------------------

/* NAF: Usualmente no me gusta unir dos puntos del pseudocodigo en la implementacion porque
siento que desmodularizo el codigo pero en este caso particular como es tan ineficiente la 
forma en que el proceso padre recive los datos lo mejor es procesarlos de una vez y no tener
que irlos guardando todos*/

if (ret != 0){//Para procesos hijos
	exit(res);//Retornamos el tablero con los daños
}
else{
	int afect[nobj];//Arreglo de booleanos que dice si un objetivo fue afectado por una bomba
	i = 0;//Reciclamos la variable del ciclo anterior
	while (i < nobj){//Inicializamos todos los elementos en false
		afect[i] = 0;
		i = i + 1;
	}
	int dest[nobj];//Arreglo de booleanos que dice si un objetivo fue destruido por una bomba
	i = 0;//Reciclamos la variable del ciclo anterior
	while (i < nobj){//Inicializamos todos los elementos en false
		dest[i] = 0;
		i = i + 1;
	}
	int ompd = 0;//Objetivos militares parcialmente destruidos
	int omtd = 0;//Objetivos militares totalmente destruidos
	int ocpd = 0;//Objetivos civiles parcialmente destruidos
	int octd = 0;//Objetivos civiles totalmente destruidos
	i = 0;//Reciclamos el contador del ciclo anterior
	//----> NAF: Esto de recorrer y sumar daños deberia ir en una funcion <------
	while (i < size){//Recorremos el resultado buscando los daños
		j = 0;//Reciclamos el contador del ciclo anterior
		while (j < size){
			if (res[i][j] != -1){//Si el cuadro en el que estamos no es -1
				if (objs[cdb[i][j]].tipo == "c"){//si el objetivo en el que estamos es civil
					objs[cdb[i][j]].val = objs[cdb[i][j]].val - res[i][j];//Restamos los daños causados por las bombas
					if(afec[cdb[i][j]]){//Si el objetivo actual no habia sido afectado
						ocpd = ocpd + 1;//Aumentamos en uno los objetivos civiles parcialmente destruidos
						afec[cdb[i][j]] = 1;//Marcamos el objetivo como afectado
					}
					if(objs[cdb[i][j]].val <= 0 && dest[cdb[i][j]] == 0){//Si el valor de un objetivo llega a 0
						ocpd = ocpd - 1;//Restamos uno a la cuenta de parcialmente destruidos
						octd = octd + 1;//Sumamos uno a la cuenta de totalmente destruidos
						dest[cdb[i][j]] = 1;//Marcamos el objetivo como destruido
					}
				}
				else{//Si el objetivo es de tipo militar
					objs[cdb[i][j]].val = objs[cdb[i][j]].val + res[i][j];//Sumamos los daños causados por las bombas
					if(afec[cdb[i][j]]){//Si el objetivo actual no habia sido afectado
						ompd = ompd + 1;//Aumentamos en uno los objetivos militares parcialmente destruidos
						afec[cdb[i][j]] = 1;//Marcamos el objetivo como afectado
					}
					if(objs[cdb[i][j]].val >= 0 && dest[cdb[i][j]] == 0){//Si el valor de un objetivo pasa de 0
						ompd = ompd - 1;//Restamos uno a la cuenta de parcialmente destruidos
						omtd = omtd + 1;//Sumamos uno a la cuenta de totalmente destruidos
						dest[cdb[i][j]] = 1;//Marcamos el objetivo como destruido
					}
				}	
			}
			j = j + 1;
		}
		i = i + 1;
	}
	k = 0;
	while (k < n){
	// -----> NAF: no estoy claro de la sintaxis de esta linea <--------
		res = wait(0);//Recivimos el resultado de algun hijo
		i = 0;//Reciclamos el contador del ciclo anterior
		//----> NAF: Esto de recorrer y sumar daños deberia ir en una funcion <------
		while (i < size){//Recorremos el resultado buscando los daños
			j = 0;//Reciclamos el contador del ciclo anterior
			while (j < size){
				if (res[i][j] != -1){//Si el cuadro en el que estamos no es -1
					if (objs[cdb[i][j]].tipo == "c"){//si el objetivo en el que estamos es civil
						objs[cdb[i][j]].val = objs[cdb[i][j]].val - res[i][j];//Restamos los daños causados por las bombas
						if(afec[cdb[i][j]]){//Si el objetivo actual no habia sido afectado
							ocpd = ocpd + 1;//Aumentamos en uno los objetivos civiles parcialmente destruidos
							afec[cdb[i][j]] = 1;//Marcamos el objetivo como afectado
						}
						if(objs[cdb[i][j]].val <= 0 && dest[cdb[i][j]] == 0){//Si el valor de un objetivo llega a 0
							ocpd = ocpd - 1;//Restamos uno a la cuenta de parcialmente destruidos
							octd = octd + 1;//Sumamos uno a la cuenta de totalmente destruidos
							dest[cdb[i][j]] = 1;//Marcamos el objetivo como destruido
						}
					}
					else{//Si el objetivo es de tipo militar
						objs[cdb[i][j]].val = objs[cdb[i][j]].val + res[i][j];//Sumamos los daños causados por las bombas
						if(afec[cdb[i][j]]){//Si el objetivo actual no habia sido afectado
							ompd = ompd + 1;//Aumentamos en uno los objetivos militares parcialmente destruidos
							afec[cdb[i][j]] = 1;//Marcamos el objetivo como afectado
						}
						if(objs[cdb[i][j]].val >= 0 && dest[cdb[i][j]] == 0){//Si el valor de un objetivo pasa de 0
							ompd = ompd - 1;//Restamos uno a la cuenta de parcialmente destruidos
							omtd = omtd + 1;//Sumamos uno a la cuenta de totalmente destruidos
							dest[cdb[i][j]] = 1;//Marcamos el objetivo como destruido
						}
					}	
				}
				j = j + 1;
			}
			i = i + 1;
		}
	k = k + 1;
	}
}
//------------------------------------------------------------------(9)------------------------------------------------------------------