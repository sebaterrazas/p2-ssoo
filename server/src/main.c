#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "comunication.h"
#include "conection.h"

char * revert(char * message){
  //Se invierte el mensaje
  
  int len = strlen(message) + 1;
  char * response = malloc(len);

  for (int i = 0; i < len-1; i++)
  {
    response[i] = message[len-2-i];
  }
  response[len-1] = '\0';
  return response;
}

int main(int argc, char *argv[]){
  // Se define una IP y un puerto
  char * IP = (char *)argv[2];
  int PORT = atoi(argv[4]);
  int MAX_CLIENTS = 20;

  // Se crea el servidor
  int server_socket = setup_server(IP, PORT); 
  fd_set current_sockets, ready_sockets;

  // Se inicializa el conjunto de descriptores de archivos
  FD_ZERO(&current_sockets);
  FD_SET(server_socket, &current_sockets);

  User** current_users = calloc(MAX_CLIENTS, sizeof(User*));
  // Se inicializa la lista con valores NULL
  for (int i = 0; i < MAX_CLIENTS; i++) {
    current_users[i] = NULL;
  }

  Room** rooms_list = calloc(MAX_CLIENTS/2, sizeof(Room*));
  // Se inicializa la lista con Salas vacías
  for (int i = 0; i < MAX_CLIENTS/2; i++) {
    Room* room = malloc(sizeof(Room));
    room->client1 = NULL;
    room->client2 = NULL;
    room->room_id = i;
    room->occupied_by = 0;
    rooms_list[i] = room;
  }

  while (1) {
    ready_sockets = current_sockets;

    if (select(FD_SETSIZE, &ready_sockets, NULL, NULL, NULL) < 0) {
      perror("select");
      exit(EXIT_FAILURE);
    }

    // Se revisa cada socket para ver si hay algo que leer
    for (int i = 0; i < FD_SETSIZE; i++) {
      if (FD_ISSET(i, &ready_sockets)) {
        if (i == server_socket) {
          // Se acepta una nueva conexión
          int client_socket = accept_new_connection(server_socket);
          int num_users;
          for (num_users = 0; num_users < MAX_CLIENTS; num_users++) {
            if (current_users[num_users] == NULL) {
              break;
            }
          }
          if (num_users >= MAX_CLIENTS) {
            server_send_message(client_socket, 1, "Servidor lleno");
            continue;
          }
          server_send_message(client_socket, 1, "Bienvenido al servidor, ¿cuál es tu nombre?");
          User* client_user = malloc(sizeof(User));
          client_user->socket = client_socket;
          client_user->name = NULL;
          client_user->phase = "login";
          client_user->status = "online";
          int user_id;
          for (user_id = 0; user_id < MAX_CLIENTS; user_id++) {
            if (current_users[user_id] == NULL) {
              break;
            }
          }
          client_user->id = user_id;
          current_users[user_id] = client_user;
          FD_SET(client_socket, &current_sockets);
        } else {
          // Se maneja una conexión existente
          bool exit = handle_communication(i, current_users, rooms_list, MAX_CLIENTS);
          if (exit) FD_CLR(i, &current_sockets);
        }
      }
    }
    printf("------------------\n");
  }
  free_memory(current_users, rooms_list, MAX_CLIENTS);

  return 0;
}
