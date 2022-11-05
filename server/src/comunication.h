#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

int server_receive_id(int client_socket);
char * server_receive_payload(int client_socket);
void server_send_message(int client_socket, int pkg_id, char * message);
void server_send_image(int client_socket, int pkg_id, char * image_route);