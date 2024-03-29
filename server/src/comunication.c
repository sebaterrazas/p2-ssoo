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
  int size = strlen(message) + 1;

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
    char msg[1+1+bytes_to_send];
    msg[0] = pkg_id;
    msg[1] = bytes_to_send;
    memcpy(&msg[2], &message[bytes_sent], bytes_to_send);
    // Se envía el paquete
    send(client_socket, msg, 2+bytes_to_send, 0);
    bytes_sent += bytes_to_send;
  }
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
  fclose(image_file);
}

bool handle_communication(int client_socket, User** current_users, Room** rooms_list, int MAX_CLIENTS) {
  int user_id = 0;
  for (user_id; user_id < MAX_CLIENTS; user_id++) {
    if (current_users[user_id]->socket == client_socket) break;
  }
  User* client_user = current_users[user_id];
  int msg_code = server_receive_id(client_socket);
  char* client_message = server_receive_payload(client_socket);
  printf("El cliente %d dice: %s\n", client_socket, client_message);

  char response[500];
  strcpy(response, "");
  if (strcmp(client_message, "exit")==0) {
    client_user->status = "offline";
    // Le enviamos la respuesta
    server_send_message(client_socket, 1, "¡Hasta luego!");
    return true;
  }
  if (strcmp(client_message, "force exit")==0) {
    client_user->status = "offline";
    return true;
  }
  // Si el usuario no tiene nombre, entonces este es el mensaje preguntandole por el nombre
  // Entonces se lo asignamos y le mostramos el lobby. 
  if (client_user->name==NULL) {
    User* old_user = check_username(client_message, current_users, MAX_CLIENTS);
    if (old_user != NULL) {
      if (strcmp(old_user->status, "offline")!=0) {
        strcat(response, "El nombre de usuario ya está en uso, por favor elija otro.");
      } else {
        current_users[client_user->id] = NULL;
        client_user->id = old_user->id;
        client_user->name = old_user->name;
        client_user->phase = old_user->phase;
        client_user->status = "online";
        current_users[old_user->id] = client_user;
        free(old_user);
        strcat(response, reconnect_msg(client_user, current_users, rooms_list, MAX_CLIENTS));
      }
    } else {
      strcat(response, "Bienvenido al lobby\n\n");
      client_user->name = client_message;
      client_user->phase = "lobby";
      strcat(response, mostar_lobby(current_users, rooms_list, MAX_CLIENTS));
    }
  }
  // Si el usuario está en el lobby, y apretó enter (mensaje vacío) entonces quería actualizar el lobby
  else if (strcmp(client_message, "")==0 && strcmp(client_user->phase, "lobby")==0) {
    strcat(response, mostar_lobby(current_users, rooms_list, MAX_CLIENTS));
  }
  // Le enviamos la respuesta
  server_send_message(client_socket, 1, response);
  return false;
}

char* mostar_lobby(User** current_users, Room** rooms_list, int MAX_CLIENTS) {
  char* lobby = malloc(500);
  strcpy(lobby, "Usuarios:\n");
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (current_users[i] != NULL && strcmp(current_users[i]->phase, "login")!=0) {

      char user[50];
      strcpy(user, "\t- ");
      strcat(user, current_users[i]->name);
      strcat(user, " [ ");
      strcat(user, current_users[i]->phase);
      strcat(user, " | ");
      strcat(user, current_users[i]->status);
      strcat(user, " ]\n");
      strcat(lobby, user);
    }
  }
  strcat(lobby, "\nSalas:\n");
  for (int i = 0; i < MAX_CLIENTS/2; i++) {
    if (rooms_list[i] != NULL) {
      char room_num[10];
      sprintf(room_num, "%d", rooms_list[i]->room_id);
      char num_clients[10];
      sprintf(num_clients, "%d", rooms_list[i]->occupied_by);

      char room[50];
      strcpy(room, "\t- Sala ");
      strcat(room, room_num);
      strcat(room, " (");
      strcat(room, num_clients);
      strcat(room, " / 2)\n");
      strcat(lobby, room);
    }
  }
  strcat(lobby, "\nApreta un número para unirte a esa sala, o\napreta Enter para actualizar el lobby.");
  return lobby;
}

User* check_username(char* username, User** current_users, int MAX_CLIENTS) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (current_users[i] != NULL && current_users[i]->name != NULL) {
      if (strcmp(current_users[i]->name, username)==0) {
        return current_users[i];
      }
    }
  }
  return NULL;
}

char* reconnect_msg(User* client_user, User** current_users, Room** rooms_list, int MAX_CLIENTS) {
  char* response = malloc(500);
  strcpy(response, "Bienvenido de vuelta, ");
  strcat(response, client_user->name);
  strcat(response, "\n\n");
  strcat(response, mostar_lobby(current_users, rooms_list, MAX_CLIENTS));
  return response;
}

void free_memory(User** current_users, Room** rooms_list, int MAX_CLIENTS) {
  for (int i = 0; i < MAX_CLIENTS; i++) {
    if (current_users[i] != NULL) {
      free(current_users[i]);
    }
  }
  free(current_users);

  for (int i = 0; i < MAX_CLIENTS/2; i++) {
    if (rooms_list[i] != NULL) {
      free(rooms_list[i]);
    }
  }
  free(rooms_list);
}
