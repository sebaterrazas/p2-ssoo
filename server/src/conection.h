#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include "comunication.h"
#pragma once

typedef struct players_info{
  int socket_c1;
  int socket_c2;
} PlayersInfo;

int setup_server(char* IP, int port);
int accept_new_connection(int server_socket);
int check(int exp, const char *msg);
