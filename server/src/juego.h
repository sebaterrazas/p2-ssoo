#pragma once
#include "comunication.h"

char* mostrar_tablero(User* user);
void iniciar_tablero(Room* room);
bool validar_coordenadas(char horizontal, int barco, char coordenada_x, char coordenada_y, User* user);
void colocar_barco(bool horizontal, int barco, int x, int y, User* user);
char* pedir_coordenadas(int barco);
char* pedir_disparo();
char* verificar_disparo(User* user, char x, char y);
char* pedir_confirmacion();

// Variables
bool horizontal;
int ALTO = 5;
int ANCHO = 5;
char coordenada_x;
char coordenada_y;
int x;
int y;

