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

  char msg[1+1+3];
  msg[0] = pkg_id;
  msg[1] = 3;
  memcpy(&msg[2], &size, 3);
  send(client_socket, msg, 5, 0);
  int bytes_sent = 0;
  while (bytes_sent != size) {
    int bytes_to_send = size - bytes_sent;
    if (bytes_to_send > 255) {
      bytes_to_send = 255;
    }
    char buffer[bytes_to_send];
    fread(buffer, bytes_to_send, 1, image_file);
    char msg[1+1+bytes_to_send];
    msg[0] = pkg_id;
    msg[1] = bytes_to_send;
    memcpy(&msg[2], buffer, bytes_to_send);
    // Se envía el paquete
    send(client_socket, msg, 2+bytes_to_send, 0);
    bytes_sent += bytes_to_send;
  }
}
