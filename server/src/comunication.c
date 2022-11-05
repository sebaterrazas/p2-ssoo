#include "comunication.h"

//LINKS REFERENCIAS
//https://pubs.opengroup.org/onlinepubs/009695399/functions/recv.html
//https://pubs.opengroup.org/onlinepubs/009695399/functions/send.html

int server_receive_id(int client_socket){
  // Se obtiene solamente el ID del mensaje
  int id = 0;
  recv(client_socket, &id, 1, 0);
  return id;
}

char * server_receive_payload(int client_socket){
  // Se obtiene el largo del payload
  int len = 0;
  recv(client_socket, &len, 1, 0);
  // Se obtiene el payload
  char * payload = malloc(len);
  int received = recv(client_socket, payload, len, 0);
  // Se retorna
  return payload;
}

void server_send_message(int client_socket, int pkg_id, char * message){
  int payloadSize = strlen(message) + 1;
  //printf("payload size: %d\n", payloadSize);
  // Se arma el paquete
  char msg[1+1+payloadSize];
  msg[0] = pkg_id;
  msg[1] = payloadSize;
  memcpy(&msg[2], message, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 2+payloadSize, 0);
}

void server_send_image(int client_socket, int pkg_id, char * image_route){
  FILE *image_file = fopen(image_route, "rb");
  fseek(image_file, 0, SEEK_END); // seek to end of file
  size_t size = ftell(image_file); // get current file pointer
  fseek(image_file, 0, SEEK_SET); // seek back to beginning of file
  char buffer[size];
  int succ = fread(buffer, size, 1, image_file);
  if (succ != 1) printf("error reading image file");
  int payloadSize = size;
  printf("payloadSize: %d\n", payloadSize);
  fclose(image_file);
  //printf("payload size: %d\n", payloadSize);
  // Se arma el paquete: ID (1B), payloadSize (3B), payload (payloadSize B)
  char msg[1+3+payloadSize];
  msg[0] = pkg_id;
  memcpy(&msg[1], &payloadSize, 3);
  memcpy(&msg[4], buffer, payloadSize);
  // Se envía el paquete
  send(client_socket, msg, 4+payloadSize, 0);
}
