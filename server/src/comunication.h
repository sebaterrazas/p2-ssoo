#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#pragma once

typedef struct user {
    int socket;
    char *name;
    char *phase;  // login, lobby, game
} User;

typedef struct room {
    User* client1;
    User* client2;
    int room_id;
    int occupied_by;
} Room;


int server_receive_id(int client_socket);
char * server_receive_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char * message);
void server_send_image(int client_socket, int pkg_id, char * image_route);
void handle_communication(User* client_user, User** current_users, Room** rooms_list, int MAX_CLIENTS);
char* mostar_lobby(User** current_users, Room** rooms_list, int MAX_CLIENTS);
bool username_available(char* username, User** current_users, int MAX_CLIENTS);
void free_memory(User** current_users, Room** rooms_list, int MAX_CLIENTS);