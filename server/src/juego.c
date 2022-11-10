#include "comunication.h"


extern int ANCHO; // declaration of global variable
extern int ALTO; // declaration of global variable
extern bool horizontal;
extern char coordenada_x;
extern char coordenada_y;
extern int x;
extern int y;


// Imprimir tablero
char* mostrar_tablero(User* user)
{
    char* tablero = malloc(1000);
    strcpy(tablero, "\t\t\n");
    strcat(tablero, "\t\tBATALLA NAVAL\n\n");
    strcat(tablero, "\t     A  B  C  D  E\n");
    for (int i = 0; i < ALTO; i++)
    {
        strcat(tablero, "\t");
        for (int j = 0; j < ANCHO; j++)
        {
            if (j == 0) {
                char i_str[10];
                sprintf(i_str, " %d ", i + 1);
                strcat(tablero, i_str);
            }
            if (user->room->client1 == user)
            {
                if (user->room->client2->tablero[i][j] == 2) strcat(tablero, "  X");
                else if (user->room->client2->tablero[i][j] == 3) strcat(tablero, "  -");
                else strcat(tablero, "  *");
            }
            else
            {
                if (user->room->client1->tablero[i][j] == 2) strcat(tablero, "  X");
                else if (user->room->client1->tablero[i][j] == 3) strcat(tablero, "  -");
                else strcat(tablero, "  *");
            }
        }
        strcat(tablero, "\n");
    }
    strcat(tablero, "\n\n\t___________________\n\n\n\n");
    // Tablero del jugador
    strcat(tablero, "\t     A  B  C  D  E\n");
    for (int i = 0; i < ALTO; i++)
    {
        strcat(tablero, "\t");
        for (int j = 0; j < ANCHO; j++)
        {
            if (j == 0)
            {
                char temp[15];
                sprintf(temp, " %d ", i + 1);
                strcat(tablero, temp);
            }
            if (user->tablero[i][j] == 1) strcat(tablero, "  O");
            else if (user->tablero[i][j] == 2) strcat(tablero, "  X");
            else if (user->tablero[i][j] == 3) strcat(tablero, "  -");
            else strcat(tablero, "  *");
        }
        if (i == 3) strcat(tablero, "\t\t\t TU");
        else if (i == 1) 
        {
            strcat(tablero, "\t\t\t Puntaje: ");
            char puntaje[2];
            sprintf(puntaje, "%d", user->puntaje);
            strcat(tablero, puntaje);
        }
        strcat(tablero, "\n");
    }
    strcat(tablero, "\n\n");
    return tablero;
}

void iniciar_tablero(Room* room)
{
    for (int i = 0; i < ALTO; i++)
    {
        for (int j = 0; j < ANCHO; j++)
        {
            room->client1->tablero[i][j] = 0;
            room->client1->tablero_barcos[i][j] = 0;
            room->client2->tablero[i][j] = 0;
            room->client2->tablero_barcos[i][j] = 0;
        }
    }
    for (int i = 0; i < 5; i++)
    {
        room->client1->barcos[i] = 0;
        room->client2->barcos[i] = 0;
    }
}

void colocar_barco(bool horizontal, int barco, int x, int y, User* user)
{
    if (horizontal)
    {
        for (int i = x; i < x + barco; i++)
        {
            user->tablero[y][i] = 1;
            user->tablero_barcos[y][i] = barco;
        }
    }
    else
    {
        for (int i = y; i < y + barco; i++)
        {
            user->tablero[i][x] = 1;
            user->tablero_barcos[i][x] = barco;
        }
    }
    // ???????
    user->barcos[barco - 1] = barco;
}

bool validar_coordenadas(char horizontal1, int barco, char coordenada_x, char coordenada_y, User* user)
{
    if (horizontal1 == '1') horizontal = true;
    else horizontal = false;
    if (coordenada_x == 'a' || coordenada_x == 'A') x = 0;
    else if (coordenada_x == 'b' || coordenada_x == 'B') x = 1;
    else if (coordenada_x == 'c' || coordenada_x == 'C') x = 2;
    else if (coordenada_x == 'd' || coordenada_x == 'D') x = 3;
    else if (coordenada_x == 'e' || coordenada_x == 'E') x = 4;
    y = coordenada_y - '0' - 1;

    if (horizontal)
    {
        if (x + barco > ANCHO) return false;
        for (int i = x; i < x + barco; i++)
        {
            if (user->tablero[y][i] != 0) return false;
        }
    }
    else
    {
        if (y + barco > ALTO) return false;
        for (int i = y; i < y + barco; i++)
        {
            if (user->tablero[i][x] != 0) return false;
        }
    }
    colocar_barco(horizontal, barco, x, y, user);
    return true;
}

char* pedir_coordenadas(int barco)
{
    char* peticion = malloc(1000);
    strcpy(peticion, "\t\t\n");
    strcat(peticion, "  Escoge la orientación y posición de inicio de tu barco de largo ");
    // char largo = str(barco);
    char largo[15];
    sprintf(largo, "%d", barco);
    strcat(peticion, largo);
    strcat(peticion, "\n");
    strcat(peticion, "    - '1' para horizontal\n");
    strcat(peticion, "    - '2' para vertical\n");
    strcat(peticion, "  El formato es el siguiente: \n");
    strcat(peticion, "    (1 o 2),X,Y\n");
    strcat(peticion, "  Donde X es la coordenada horizontal y Y la vertical\n");
    return peticion;
}

char* pedir_disparo()
{
    char* peticion = malloc(1000);
    strcat(peticion, "  Elige las coordenadas de tu disparo: \n");
    strcat(peticion, "    - El formato es el siguiente: \n");
    strcat(peticion, "      X,Y\n");
    strcat(peticion, "    - Donde X es la coordenada horizontal y Y la vertical\n");
}

char* verificar_disparo(User* user, char coordenada_x, char coordenada_y)
{
    char* resultado = malloc(1000);
    if (coordenada_x == 'A' || coordenada_x == 'a') x = 0;
    else if (coordenada_x == 'B' || coordenada_x == 'b') x = 1;
    else if (coordenada_x == 'C' || coordenada_x == 'c') x = 2;
    else if (coordenada_x == 'D' || coordenada_x == 'd') x = 3;
    else if (coordenada_x == 'E' || coordenada_x == 'e') x = 4;
    y = coordenada_y - '0';
    y--;
    //////////////////////////////////////////
    if (user->room->client1 == user)
    {
        if (user->room->client2->tablero[y][x] == 0)
        {
            user->room->client2->tablero[y][x] = 3;
            strcat(resultado, "\n\n ¡Has fallado!\n\n");
        }
        else if (user->room->client2->tablero[y][x] == 1)
        {
            user->room->client2->tablero[y][x] = 2;
            user->puntaje++;
            strcat(resultado, "\n\n ¡Has acertado!\n\n");
            user->room->client2->barcos[user->room->client2->tablero_barcos[y][x] - 1]--;
            if (user->room->client2->barcos[user->room->client2->tablero_barcos[y][x] - 1] == 0)
            {
                strcat(resultado, "\n\n¡Has hundido un barco!\n\n\n");
            }
        }
        else if (user->room->client2->tablero[y][x] == 2)
        {
            strcat(resultado, "\n\n ¡Ya has disparado a esa casilla! Había un barco aquí\n\n");
        }
        else if (user->room->client2->tablero[y][x] == 3)
        {
            strcat(resultado, "\n\n ¡Ya has disparado a esa casilla! No había nada aquí\n\n");
        }
    }
    else 
    {
        if (user->room->client1->tablero[y][x] == 0)
        {
            user->room->client1->tablero[y][x] = 3;
            strcat(resultado, "\n\n ¡Has fallado!\n\n");
        }
        else if (user->room->client1->tablero[y][x] == 1)
        {
            user->room->client1->tablero[y][x] = 2;
            user->puntaje++;
            strcat(resultado, "\n\n ¡Has acertado!\n\n");
            user->room->client1->barcos[user->room->client1->tablero_barcos[y][x] - 1]--;
            if (user->room->client1->barcos[user->room->client1->tablero_barcos[y][x] - 1] == 0)
            {
                strcat(resultado, "¡Has hundido un barco!\n\n");
            }
        }
        else if (user->room->client1->tablero[y][x] == 2)
        {
            strcat(resultado, "\n\n ¡Ya has disparado a esa casilla! Había un barco aquí\n\n");
        }
        else if (user->room->client1->tablero[y][x] == 3)
        {
            strcat(resultado, "\n\n ¡Ya has disparado a esa casilla! No había nada aquí\n\n");
        }
    }
    return resultado;
}

char* pedir_confirmacion()
{
    char* peticion = malloc(26);
    strcpy(peticion, "  ¿Esta es tu flota? (s/n)\n");
    return peticion;
}