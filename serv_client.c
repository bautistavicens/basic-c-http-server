/*
  serv_client.c
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
#include "arpa/inet.h"
#include "arpa/inet.h"
#include "sys/types.h"
#include "fcntl.h"
#include "sys/stat.h"

void *serv_client(void *parametros){

	/*Variables - Socket*/
	int clientSocket = *((int *) parametros);
	int sent;

	/*Variables - Geberales*/
	char buffer[2048];
	char sendBuffer[1024];
	char responseHeaders[1024]; 
	size_t blockSize = 1024;
	size_t sentBytes;


	/*Variables - Archivos*/
	int imageFD;
	struct stat fileStats;
	off_t fileSize;

	/*****************************Traigo Archivo*******************************/
	/*Traigo la imagen*/
	if((imageFD = open(IMG_FILE_PATH, O_RDONLY)) < 0){
		printf("\nERROR (Hilo).\nNo se puedo abrir la imagen");
		close(clientSocket);
		pthread_exit(NULL);
	}

	/*Cargo stats*/
	fstat(imageFD, &fileStats);

	fileSize = fileStats.st_size;
	/*************************************************************************/


	/*Creo espacio en memoria para responseHeaders*/
	memset(responseHeaders, 0x00, sizeof(responseHeaders));


	/*Muestro mensaje de conexión establecida*/
	printf("\n\n Conexón establecida en socket: #%d !\n", clientSocket);
	
	/*Duermo el Hilo 3 segundos*/
	sleep(3);
	
	/***************************** Request *******************************/
	/*Asigno memoria al buffer*/
	memset(buffer, 0x00, 2048);

	/*Recibo la request*/
	recv(clientSocket, &buffer, 2048, 0);
	/***********************************************************************/
	
	/*Verificio que la URI sea "/imagen.png" */
	if (strstr(buffer, "GET /imagen.png") != NULL) {
		printf("\n\nRequest: %s\n", buffer);

		/*Duermo el hilo 2 segundos para mostrar request.*/
		sleep(2);
		
		/*Muestro mensaje con stats*/
		printf("\n***************************************");
		printf("\n\t\tArchivo\n");
		printf("\nNombre: %s", IMG_NAME);
		printf("\nTamaño: %ld", fileStats.st_size);
		printf("\n***************************************\n");	

		/*Duermo el hilo 5 segundos para mostrar info.*/
		sleep(5);
		
		/***************************** Response *******************************/

		/*Cargo los headers del response*/
		sprintf(responseHeaders, "HTTP/1.1 200 OK\r\nContent-type:image/png\r\nContent-lenght: %ld\r\nConnection: Close\r\n\r\n", fileStats.st_size);
		
		/*Envio response*/
		sent = send(clientSocket, responseHeaders, strlen(responseHeaders), 0);
		
		/**********************************************************************/


		/*Muestro mensaje de comienzo de transacción*/
		printf("\n\n Enviando archivo...\n\n");

		/*Duermo el hilo 2 Segundo*/
		sleep(2);

		while (fileSize > 0){

			if(fileSize < 1024){
				blockSize = fileSize;
					
			}

		/*Asigno un tamaño de 1024 bytes*/
		memset(sendBuffer, 0X00, 1024);

		/*Leo el archivo (imagen)*/
		read(imageFD, &sendBuffer, blockSize);
	
		sentBytes = send(clientSocket, &sendBuffer, blockSize, 0);
	
		fileSize = fileSize - sentBytes;
	
		printf("\nBytes restantes: %ld", fileSize);

		usleep(60000);
	
		}

	/*Muestro el response por consola*/
	printf("\n\nResponse header enviado a cliente:\n\n");
	printf("%s", responseHeaders);
	}
	/* URI no es"/imagen.png" */
	else{

		/***************************** Response *******************************/
		/*Cargo los headers del response*/
		strcpy(responseHeaders,"HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\nConnection: Close\r\n\r\n");
		/*Envio response*/
		send(clientSocket, responseHeaders, strlen(responseHeaders), 0);
		/**********************************************************************/

		/*Muestro el response por consola*/
		printf("\n\nResponse header enviado a cliente:\n\n");
		printf("%s", responseHeaders);

	}

	/*Cierro los File Descriptors*/
	close(clientSocket);
	close(imageFD);
	
	/*Regreso al hilo principal*/
	pthread_exit((void*)NULL);

}
