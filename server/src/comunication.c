#include "comunication.h"
#include "juego.h"

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
  printf("El cliente %d (%s) dice: %s\n", client_socket, client_user->phase, client_message);
  char* filename = "welldone.jpg";

  // server_send_image(client_socket, 0, filename);

  char response[1500];
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
        client_user->room = old_user->room;
        client_user->tablero = old_user->tablero;
        client_user->tablero_barcos = old_user->tablero_barcos;
        client_user->barcos = old_user->barcos;
        client_user->puntaje = old_user->puntaje;
        current_users[old_user->id] = client_user;
        free(old_user);

        char* reconnect = reconnect_msg(client_user, current_users, rooms_list, MAX_CLIENTS);
        strcat(response, reconnect);
        free(reconnect);
      }
    } else {
      strcat(response, "Bienvenido al lobby\n\n");
      client_user->name = client_message;
      client_user->phase = "lobby";
      char* lobby = mostrar_lobby(current_users, rooms_list, MAX_CLIENTS);
      strcat(response, lobby);
      free(lobby);
    }
  }
  // Si el usuario está en el lobby, y apretó enter (mensaje vacío) entonces quería actualizar el lobby
  else if (strcmp(client_message, "")==0 && strcmp(client_user->phase, "lobby")==0) {
    char* lobby = mostrar_lobby(current_users, rooms_list, MAX_CLIENTS);
    strcat(response, lobby);
    free(lobby);
  }
  ///Tratar de entrar a sala
  else if (strcmp(client_user->phase, "lobby")==0) {
    // Ver qué el mensaje del cliente sea un número de sala
    bool valido = false;
    int room_id;
    for (int i = 0; i < MAX_CLIENTS/2; i++)
    {
      char i_str[10];
      sprintf(i_str, "%d", i);
      if (strcmp(client_message, i_str)==0) {
        valido = true;
        room_id = i;
        break;
      }
    }
    if (valido) {
      Room* room = rooms_list[room_id];
      if (room != NULL) {
        // Si la sala está llena, entonces no se puede entrar
        if (room->client1 && room->client2) {
          strcat(response, "La sala está llena, por favor elija otra.");
        } else {
          if (room->client1 == NULL) {
            room->client1 = client_user;
          } else {
            room->client2 = client_user;
          }
          room->occupied_by++;
          client_user->phase = "room";
          client_user->room = room;
          // mensaje "Bienvenido a la sala {room_id} {nombre_cliente}"
          strcat(response, "Bienvenido a la sala ");
          char room_id_str[10];
          sprintf(room_id_str, "%d", room_id);
          strcat(response, room_id_str);
          strcat(response, " ");
          strcat(response, client_user->name);
          strcat(response, "\n");
        }
        if (room->client1 && room->client2) {
          // Hay clientes suficientes para empezar la partida
          strcat(response, "Sala lista.\nEscriba 'ready' para decir que está listo para empezar la partida,\no 'salir' para salir de la sala.");
        }
        else
        {
          // No hay clientes suficientes para empezar la partida
          strcat(response, "Esperando a otro jugador...\nApreta Enter actualizar la sala,\no escribe 'salir' para salir de la sala.");
        }
      }
    }
  }
  else if (strcmp(client_user->phase, "room")==0 && strcmp(client_message, "")==0) {
    Room* room = client_user->room;
    if (room->client1 && room->client2) {
    // Hay clientes suficientes para empezar la partida
    // Se envía el opcion de empezar partida
      if (room->p1_ready && room->p2_ready)
      {
        room->client1->phase = "choosing4";
        room->client2->phase = "choosing4";
        char* tablero = mostrar_tablero(client_user);
        strcpy(response, tablero);
        free(tablero);
        char* peticion = pedir_coordenadas(4);
        strcat(response, peticion);
        free(peticion);
      }
      else 
      {
        if (room->client1 == client_user)
        {
          if (room->p1_ready) strcpy(response, "Esperando a que el jugador 2 esté listo. Presione Enter para actualizar.");
          else strcpy(response, "Escriba 'ready' para decir que está listo para empezar la partida,\no 'salir' para salir de la sala.");
        }
        else if (room->client2 == client_user)
        {
          if (room->p2_ready) strcpy(response, "Esperando a que el jugador 1 esté listo. Presione Enter para actualizar.");
          else strcpy(response, "Escriba 'ready' para decir que está listo para empezar la partida,\no 'salir' para salir de la sala.");
        }
        else strcpy(response, "Algo raro");
      }
    }
    else strcpy(response, "Esperando a otro jugador...\nApreta Enter actualizar la sala,\no escribe 'salir' para salir de la sala.");
  }
  else if (strcmp(client_user->phase, "room")==0 && strcmp(client_message, "salir")==0) {
    Room* room = client_user->room;
    if (room->client1 == client_user) {
      room->client1 = NULL;
    } else {
      room->client2 = NULL;
    }
    room->occupied_by--;
    client_user->phase = "lobby";
    client_user->room = NULL;
    strcat(response, "Salió de la sala.\n");
    char* lobby = mostrar_lobby(current_users, rooms_list, MAX_CLIENTS);
    strcat(response, lobby);
    free(lobby);
  }
  else if (strcmp(client_user->phase, "room")==0 && strcmp(client_message, "ready")==0){
    Room* room = client_user->room;
    if (room->client1 == client_user) {
      room->p1_ready = true;
    } else {
      room->p2_ready = true;
    }
    //Despues ver si imprime al otro jugador status de sala (yo ready)
    if ((room->p1_ready && !room->p2_ready) || (!room->p1_ready && room->p2_ready)) {
      strcat(response, "Esperando al otro jugador...\nApreta Enter para actualizar.");
    }
    if (room->p1_ready && room->p2_ready){
      // mostrar el tablero
      // cambiar fase de los jugadores a "choosing"
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      char* peticion = pedir_coordenadas(4);
      strcat(response, peticion);
      free(peticion);
      room->client1->phase = "choosing4";
      room->client2->phase = "choosing4";
    }
  }
  else if (strcmp(client_user->phase, "choosing4")==0) {
    // verificar calidad del mensaje
    // largo del mensaje = 5
    // mensaje[0] = 1 o 2
    // mensaje[1] = ","
    // mensaje[2] = letra "A", "a", "B", "b", "C", "c", "D", "d", "E", "e"
    // mensaje[3] = ","
    // mensaje[4] = numero "1", "2", "3", "4", "5"
    // Verificar
    if (strlen(client_message) == 0) {
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      char* peticion = pedir_coordenadas(4);
      strcat(response, peticion);
    }
    else if (strlen(client_message) == 5) {
      char horizontal = client_message[0];
      char coordenada_x = client_message[2];
      char coordenada_y = client_message[4];
      if(
        (horizontal == '1' ||
         horizontal == '2') &&
        (coordenada_x == 'A' || coordenada_x == 'a' ||
         coordenada_x == 'B' || coordenada_x == 'b' ||
         coordenada_x == 'C' || coordenada_x == 'c' ||
         coordenada_x == 'D' || coordenada_x == 'd' ||
         coordenada_x == 'E' || coordenada_x == 'e') &&
        (coordenada_y == '1' || coordenada_y == '2' ||
         coordenada_y == '3' || coordenada_y == '4' ||
         coordenada_y == '5'))
      {
        if (validar_coordenadas(horizontal, 4, coordenada_x, coordenada_y, client_user))
        {
          client_user->phase = "choosing3";
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          char* peticion = pedir_coordenadas(3);
          strcat(response, peticion);
          free(peticion);
        }
        else
        {
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
          char* peticion = pedir_coordenadas(4);
          strcat(response, peticion);
          free(peticion);
        }
      }
      else
      {
        char* tablero = mostrar_tablero(client_user);
        strcpy(response, tablero);
        free(tablero);
        strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
        char* peticion = pedir_coordenadas(4);
        strcat(response, peticion);
        free(peticion);
      }
    }
    else {
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
      char* peticion = pedir_coordenadas(4);
      strcat(response, peticion);
      free(peticion);
    }
  }
  else if (strcmp(client_user->phase, "choosing3")==0) {
    // verificar calidad del mensaje
    // largo del mensaje = 5
    // mensaje[0] = 1 o 2
    // mensaje[1] = ","
    // mensaje[2] = letra "A", "a", "B", "b", "C", "c", "D", "d", "E", "e"
    // mensaje[3] = ","
    // mensaje[4] = numero "1", "2", "3", "4", "5"
    // Verificar
    if (strlen(client_message) == 5)
    {
      char horizontal = client_message[0];
      char coordenada_x = client_message[2];
      char coordenada_y = client_message[4];
      if (
        (horizontal == '1' ||
         horizontal == '2') &&
        (coordenada_x == 'A' || coordenada_x == 'a' ||
         coordenada_x == 'B' || coordenada_x == 'b' ||
         coordenada_x == 'C' || coordenada_x == 'c' ||
         coordenada_x == 'D' || coordenada_x == 'd' ||
         coordenada_x == 'E' || coordenada_x == 'e') &&
        (coordenada_y == '1' || coordenada_y == '2' ||
         coordenada_y == '3' || coordenada_y == '4' ||
         coordenada_y == '5'))
      {

        if (validar_coordenadas(horizontal, 3, coordenada_x, coordenada_y, client_user))
        {
          client_user->phase = "choosing2";
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          char* peticion = pedir_coordenadas(2);
          strcat(response, peticion);
          free(peticion);
        }
        else
        {
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
          char* peticion = pedir_coordenadas(3);
          strcat(response, peticion);
          free(peticion);
        }
      }
    }
    else
    {
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
      char* peticion = pedir_coordenadas(3);
      strcat(response, peticion);
      free(peticion);
    }
  }
  else if (strcmp(client_user->phase, "choosing2")==0){
    // verificar calidad del mensaje
    // largo del mensaje = 5
    // mensaje[0] = 1 o 2
    // mensaje[1] = ","
    // mensaje[2] = letra "A", "a", "B", "b", "C", "c", "D", "d", "E", "e"
    // mensaje[3] = ","
    // mensaje[4] = numero "1", "2", "3", "4", "5"
    // Verificar
    if (strlen(client_message) == 5)
    {
      char horizontal = client_message[0];
      char coordenada_x = client_message[2];
      char coordenada_y = client_message[4];
      if (
        (horizontal == '1' ||
         horizontal == '2') &&
        (coordenada_x == 'A' || coordenada_x == 'a' ||
         coordenada_x == 'B' || coordenada_x == 'b' ||
         coordenada_x == 'C' || coordenada_x == 'c' ||
         coordenada_x == 'D' || coordenada_x == 'd' ||
         coordenada_x == 'E' || coordenada_x == 'e') &&
        (coordenada_y == '1' || coordenada_y == '2' ||
         coordenada_y == '3' || coordenada_y == '4' ||
         coordenada_y == '5'))
      {
        if (validar_coordenadas(horizontal, 2, coordenada_x, coordenada_y, client_user))
        {
          client_user->phase = "confirm";
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          char* peticion = pedir_confirmacion();
          strcat(response, peticion);
          free(peticion);
        }
        else
        {
          char* tablero = mostrar_tablero(client_user);
          strcpy(response, tablero);
          free(tablero);
          strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
          char* peticion = pedir_coordenadas(2);
          strcat(response, peticion);
          free(peticion);
        }
      }
    }
    else
    {
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
      char* peticion = pedir_coordenadas(2);
      strcat(response, peticion);
      free(peticion);
    }
  }
  else if (strcmp(client_user->phase, "confirm")==0){
    //Si en CONFIRMAR respuesta si:
    if (strcmp(client_message ,"s") == 0 || strcmp(client_message, "S") == 0)
    {
      client_user->phase = "waiting";
      if (client_user == client_user->room->client1)
      {
        if (client_user->room->client2->phase == "waiting")
        {
          client_user->phase = "on turn";
          client_user->room->client2->phase = "not on turn";
          strcat(response, "Es tu turno, presiona enter para continuar.\n");
        }
        else strcat(response, "Esperando al otro jugador. Apreta Enter para actualizar.");
      }
      else if (client_user == client_user->room->client2)
      {
        if (client_user->room->client1->phase == "waiting")
        {
          client_user->phase = "not on turn";
          client_user->room->client1->phase = "on turn";
          strcat(response, "Es el turno de tu oponente. Apreta Enter para actualizar.");
        }
        else strcat(response, "Esperando al otro jugador. Apreta Enter para actualizar.");
      }
    }
    //Si en CONFIRMAR respuesta no:
    else if (strcmp(client_message, "n") == 0|| strcmp(client_message, "N") == 0)
    {
      client_user->phase = "choosing4";
      // reiniciar coordenadas
      for (int i = 0; i < 5; i++)
      {
        for (int j = 0; j < 5; j++)
        {
          client_user->tablero[i][j] = 0;
          client_user->tablero_barcos[i][j] = 0;
        }
      }
      char* tablero = mostrar_tablero(client_user);
      strcpy(response, tablero);
      free(tablero);
      strcat(response, "Ingrese nuevamente las coordenadas de su barco de 4 casillas.");
      char* peticion = pedir_coordenadas(4);
      strcat(response, peticion);
      free(peticion);
    }
    else
    {
      strcat(response, "Por favor elija una opción válida.\n");
    }
  }
  else if (strcmp(client_user->phase, "waiting")==0){ // Cuando se quiere actualizar y se está esperando al oponente
    if (client_user == client_user->room->client1)
    {
      if (client_user->room->client2->phase == "waiting")
      {
        client_user->phase = "on turn";
        client_user->room->client2->phase = "not on turn";
        strcat(response, "Es tu turno, presiona enter para continuar.\n");
      }
      else strcat(response, "Esperando al otro jugador. Apreta Enter para actualizar.");
    }
    else 
    {
      if (client_user->room->client1->phase == "waiting")
      {
        client_user->phase = "not on turn";
        client_user->room->client1->phase = "on turn";
        strcat(response, "Es el turno de tu oponente. Apreta Enter para actualizar.");
      }
      else strcat(response, "Esperando al otro jugador. Apreta Enter para actualizar.");
    }
  }
  else if (strcmp(client_user->phase, "on turn") == 0){
    if (client_user->room->client1 == client_user)
    {
      if (client_user->room->client2->phase == "win")
      {
        strcat(response, "¡Has perdido!\n\n");
        client_user->phase = "lose";
      }
    }
    else if (client_user->room->client2 == client_user)
    {
      if (client_user->room->client1->phase == "win")
      {
        strcat(response, "¡Has perdido!\n\n");
        client_user->phase = "lose";
      }
    }
    char* tablero = mostrar_tablero(client_user);
    strcpy(response, tablero);
    free(tablero);
    char* peticion = pedir_disparo(client_user);
    strcat(response, peticion);
    free(peticion);
    client_user->phase = "checking turn";
  }
  else if (strcmp(client_user->phase, "not on turn")==0){
    char* tablero = mostrar_tablero(client_user);
    strcpy(response, tablero);
    free(tablero);
    strcat(response, "Esperando al otro jugador.");
  }
  else if (strcmp(client_user->phase, "checking turn")==0){
    // verificar mensaje
    // largo del mensaje = 3
    // mensaje[0] = letra "A", "a", "B", "b", "C", "c", "D", "d", "E", "e"
    // mensaje[1] = ","
    // mensaje[2] = numero "1", "2", "3", "4", "5"
    // Verificar
    if (strlen(client_message) == 3) 
    {
      char coordenada_x = client_message[0];
      char coordenada_y = client_message[2];
      if (
        (coordenada_x == 'A' || coordenada_x == 'a' ||
         coordenada_x == 'B' || coordenada_x == 'b' ||
         coordenada_x == 'C' || coordenada_x == 'c' ||
         coordenada_x == 'D' || coordenada_x == 'd' ||
         coordenada_x == 'E' || coordenada_x == 'e') &&
        (coordenada_y == '1' || coordenada_y == '2' ||
         coordenada_y == '3' || coordenada_y == '4' ||
         coordenada_y == '5'))
      {
        char coordenada_x = client_message[0];
        char coordenada_y = client_message[2];
        char* verificar = verificar_disparo(client_user, coordenada_x, coordenada_y);
        strcpy(response, verificar);
        free(verificar);
        char* tablero = mostrar_tablero(client_user);
        strcat(response, tablero);
        free(tablero);
        if (client_user->puntaje == 9)
        {
          strcat(response, "¡Has ganado!\n\n");
          server_send_image(client_user->socket, 0, "welldone.jpg");
          client_user->phase = "win";
          if (client_user == client_user->room->client1) client_user->room->client2->phase = "lose";
          else client_user->phase = "lose";
        }
      }
    }
    else
    {
      strcpy(response, mostrar_tablero(client_user));
      strcat(response, "Coordenadas inválidas, por favor ingrese nuevamente.");
      strcat(response, pedir_disparo(client_user));
    }
  }
  else if ((strcmp(client_user->phase, "win") == 0) || (strcmp(client_user->phase, "lose") == 0))
  {
    // preguntar si quiere seguir jugando
    strcpy(response, "¿Quieres seguir jugando? (s/n)\n");
    client_user->room->client1->phase = "finish";
    client_user->room->client2->phase = "finish";
  }
  else if (strcmp(client_user->phase, "finish") == 0)
  {
    client_user->room->occupied_by = 0;
    client_user->room->p1_ready = false;
    client_user->room->p2_ready = false;
    if (client_user->room->client1 == client_user) client_user->room->client1 = NULL;
    else client_user->room->client2 = NULL;
    client_user->room = NULL;
    client_user->puntaje = 0;
    if (strcmp(client_message, "s") == 0 || strcmp(client_message, "S") == 0)
    {
      client_user->phase = "lobby";
      char* lobby = mostrar_lobby(current_users, rooms_list, MAX_CLIENTS);
      strcpy(response, lobby);
      free(lobby);
    }
    else if (strcmp(client_message, "n") == 0 || strcmp(client_message, "N") == 0)
    {
      client_user->phase = "finish";        
      client_user->status = "offline";
      server_send_message(client_socket, 1, "Gracias por jugar.\n\n¡Hasta luego!");
      return true;
    }
    else
    {
      strcat(response, "Por favor elija una opción válida.\n");
    }
  }
  ////////////////////////////////
  // Le enviamos la respuesta
  server_send_message(client_socket, 1, response);
  return false;
}
char* mostrar_lobby(User** current_users, Room** rooms_list, int MAX_CLIENTS) {
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
  strcpy(response, "Bienvenido de vuelta, apreta Enter para continuar.\n");
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
