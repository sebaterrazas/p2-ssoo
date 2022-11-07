#include "comunication.h"

int client_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char * client_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len_size = 0;
  recv(client_socket, &len_size, 1, 0); // Siempre 3
  int len = 0;
  recv(client_socket, &len, len_size, 0);
  char * payload = malloc(len);
  int bytes_recieved = 0;
  while (bytes_recieved != len) {
    // Se obtiene el payload
    int buffer_id = 0;
    recv(client_socket, &buffer_id, 1, 0);
    int buffer_len = 0;
    recv(client_socket, &buffer_len, 1, 0);
    char buffer[buffer_len];
    recv(client_socket, buffer, buffer_len, 0);
    memcpy(&payload[bytes_recieved], buffer, buffer_len);
    bytes_recieved += buffer_len;
  }
  // Se retorna
  return payload;
}

char * client_receive_image(int client_socket){
  char * payload = client_receive_payload(client_socket);
  int len = strlen(payload);
  FILE *image_file = fopen("image.jpg", "wb");
  fwrite(payload, len, 1, image_file);
  fclose(image_file);
  free(payload);
  // Se retorna
  return "image.jpg downloaded";
}

void client_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1; //+1 para considerar el caracter nulo. 
  //Esto solo es válido para strings, Ustedes cuando armen sus paquetes saben exactamente cuantos bytes tiene el payload.
  
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}