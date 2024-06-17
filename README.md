# basic-c-http-server

### S.O utilizado: Ubuntu.

### Compilador: gcc-4.1.


## Instrucciones

1. Abrir terminal.
2. Ingresar a la carpeta raíz del proyecto.
3. Ejecutar make (escribír make en la terminarl y presionar enter).
4. Ejecutar server (escribír ./server y presionar enter).

   

## Server

El http server implementará una versión muy reducida del protocolo http. El cliente programado para testear el servidor será dado de baja y a partir de ahora los clientes serán los browsers del mercado. El único endpoint que expone el servidor es  HTTP GET http://direcciónIp:puerto/imagen.jpg. Un request a dicha url deberá traer el archivo de imagen completo. 

##Endpoints

GET http://localhost:3030/image.png -> Devuelve una imagen, la cual va enviando de a 1024 bytes.


(!)Cualquier otra ruta solicitada devolverá el estado "Not found", con el código de estado: "404".



