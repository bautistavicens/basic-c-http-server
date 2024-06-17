/*
  server.c
  basic-c-http-server

 Created by Bautista on 27/05/2024
  Copyright © 2024 Bautista. All rights reserved.
*/

#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "define.h"
#include "unistd.h"
#include "time.h"
#include "serv_client.h"
#include "pthread.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "sys/types.h"



int main(int argc, char *argv[]){
	/*Variables*/
	int unsigned server_port = 3030;
	int yes = 1;
	char sendBuffer[1024];
	
	/*Variables y punteros de threads*/
	pthread_t idThread;
	pthread_attr_t atributos;
	int thread_create_result;

	/*Variables y punteros de sockets*/
	int serverSocket = 0;
	int clientSocket = 0;
	socklen_t client_addr_size;

	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr; 


	/*Seed rand*/
	srand (time(NULL));


	/*Creo File Descriptor*/
	serverSocket = socket(AF_INET, SOCK_STREAM, IP_PROTOCOL);

	setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes));

	/*Asigno memoria a las estructuras addr*/
	memset(&server_addr, 0x00, sizeof(server_addr));
	memset(sendBuffer, 0x00, sizeof(sendBuffer));

	/*Agrego los datos a la estructura my_addr*/

	  /*Indico que es IPv4*/ 
	server_addr.sin_family = AF_INET;

	  /*Indico el puerto que voy a utilizar (SOCKET_PORT EN define.h)*/
	server_addr.sin_port = htons(server_port);

	  /*Indico la IP que voy a utilizar*/
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);



	/*Asocio el socket con la address*/
	if(bind(serverSocket, (struct sockaddr *)&server_addr, sizeof server_addr) < 0){
		/*Detección de errores*/
		printf("\nERROR.\nBind failed");	
		close(serverSocket);
		exit(-1);	
	};	
 



	/*Colo al socket en escucha*/
	  /* 
            "serverSocket" (socket): pasamos el socket creado.
            "5" (backlog): Cantidad de conexiones que pueden estar pendientes a aceptación
	  */
	if(listen(serverSocket, 5) < 0){
		/*Detección de errores*/
		printf("\nERROR.\nListen failed");	
		close(serverSocket);
		exit(-1);	
	}



	/*Aviso: Estado del servidor*/
	printf("\n\n**************************************\n");
	printf("\n\tServer status: ONLINE");
	printf("\n\tlistening on port %u", server_port);
	printf("\n\n**************************************\n\n");

	/*Setteo el size del client_addr*/
	client_addr_size = sizeof(client_addr);

	if ((clientSocket = accept(serverSocket, (struct sockaddr *)&client_addr, &client_addr_size)) < 0) {
		/*Si ocurre algun error*/
    		printf("\nERROR.\nFalló el accept");
    		close(clientSocket);
    		exit(-1);
	}
	

	/*Cargo atributos del thread*/
	pthread_attr_init(&atributos);
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_JOINABLE /*PTHREAD_CREATE_DETACHED*/);
	pthread_attr_setschedpolicy(&atributos, SCHED_FIFO);
	
	/*Creo thread hijo.*/
	thread_create_result = pthread_create(&idThread, &atributos, serv_client, &clientSocket);
	
	/*Manejo de errores - Creación thread*/
	if (thread_create_result != 0) {
	    fprintf(stderr, "Error al crear el hilo: %d\n", thread_create_result);
	    close(clientSocket);
	    exit(EXIT_FAILURE);
	}
	
	/*Espero al thread hijo*/
	pthread_join (idThread, NULL);
	printf("\n\n\nFin\n\n\n");

	return 0;	
}

	

