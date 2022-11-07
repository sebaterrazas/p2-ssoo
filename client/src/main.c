#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include "conection.h"
#include "comunication.h"

extern int server_socket; // Variable global, para poder mandar mensaje al manejar una señal de término

char * get_input(){
  char * response = malloc(20);
  int pos=0;
  while (1){
    char c = getchar();
    if (c == '\n') break;
    response[pos] = c;
    pos++;
  }
  response[pos] = '\0';
  return response;
}


int main (int argc, char *argv[]){
  //Se obtiene la ip y el puerto donde está escuchando el servidor (la ip y puerto de este cliente da igual)
  char * IP = (char *)argv[2];
  int PORT = atoi(argv[4]);

  // Se prepara el socket

  server_socket = prepare_socket(IP, PORT);

  // Register signal and signal handler
  signal(SIGINT, signal_callback_handler);

  // Se inicializa un loop para recibir todo tipo de paquetes y tomar una acción al respecto
  while (1){
    int msg_code = client_receive_id(server_socket);

    char * message;
    if (msg_code == 0) { //Recibimos una imagen
      message = client_receive_image(server_socket);
      printf("El servidor te ha enviado una imagen [%s]\n", message);
    }
    else {
      message = client_receive_payload(server_socket);
    }

    if (msg_code == 1) { //Recibimos un mensaje de servidor 
      if (strcmp(message, "Servidor lleno") == 0) break;
      printf("%s\n", message);
    }

    if (msg_code == 2) { //Recibimos un mensaje que proviene del otro cliente
      printf("El otro cliente dice: %s\n", message);
    }
    
    if (msg_code != 0) free(message);

    //printf("¿Qué desea hacer?\n   1)Enviar mensaje al servidor\n   2)Enviar mensaje al otro cliente\n");
    //int option = getchar() - '0';
    //getchar(); //Para capturar el "enter" que queda en el buffer de entrada stdin
    int option = 1;
    
    //printf("Ingrese su mensaje: ");
    char * response = get_input();
    client_send_message(server_socket, option, response);
    printf("------------------\n");
  }

  // Se cierra el socket
  close(server_socket);
  free(IP);

  return 0;
}
