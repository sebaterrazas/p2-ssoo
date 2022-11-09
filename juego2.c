#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// Variables
bool horizontal;
int ALTO = 5;
int ANCHO = 5;
int oponente[5][5];
int jugador[5][5];
int puntaje_oponente;
int puntaje_jugador;
char coordenada_x;
char coordenada_y;
int x;
int y;


// Imprimir tablero
void mostrar_tablero()
{
    printf("\t\t\n");
    printf("\t\tBATALLA NAVAL\n\n");
    // Tablero del oponente
    printf("\t     A  B  C  D  E\n");
    for (int i = 0; i < ALTO; i++)
    {
        printf("\t");
        for (int j = 0; j < ANCHO; j++)
        {
            if (i == 0 && j == 0) printf(" 1 ");
            if (i == 1 && j == 0) printf(" 2 ");
            if (i == 2 && j == 0) printf(" 3 ");
            if (i == 3 && j == 0) printf(" 4 ");
            if (i == 4 && j == 0) printf(" 5 ");
            if (oponente[i][j] == 3) printf("  -");
            else
            {
                if (oponente[i][j] == 2) printf("  X");
                else printf("  *");
            }
        }
        if (i == 3) printf("\t\t\t OPONENTE");
        else if (i == 1) printf("\t\t\t Puntaje oponente: %d", puntaje_oponente);
        printf("\n");
    }
    printf("\n\n\t _________________ \n\n\n\n");
    printf("\t     A  B  C  D  E\n");
    for (int i = 0; i < ALTO; i++)
    {
        printf("\t");
        for (int j = 0; j < ANCHO; j++)
        {
            if (i == 0 && j == 0) printf(" 1 ");
            if (i == 1 && j == 0) printf(" 2 ");
            if (i == 2 && j == 0) printf(" 3 ");
            if (i == 3 && j == 0) printf(" 4 ");
            if (i == 4 && j == 0) printf(" 5 ");
            if (jugador[i][j] == 3) printf("  -");
            else
            {
                if (jugador[i][j] == 2) printf("  X");
                else if (jugador[i][j] == 1) printf("  O");
                else printf("  *");
            }
        }
        if (i == 3) printf("\t\t\t JUGADOR");
        else if (i == 1) printf("\t\t\t Puntaje jugador: %d", puntaje_jugador);
        printf("\n");
    }
    printf("\n\n");
}

void iniciar_tablero()
{
    for (int i = 0; i < ALTO; i++)
    {
        for (int j = 0; j < ANCHO; j++)
        {
            jugador[i][j] = 0;
            oponente[i][j] = 0;
        }
    }
}

bool validar_coordenadas(bool horizontal, int barco, int x, int y)
{
    if (horizontal)
    {
        if (x + barco > ANCHO) return false;
        for (int i = x; i < x + barco; i++)
        {
            if (jugador[y][i] != 0) return false;
        }
    }
    else
    {
        if (y + barco > ALTO) return false;
        for (int i = y; i < y + barco; i++)
        {
            if (jugador[i][x] != 0) return false;
        }
    }
    return true;
}

// // retorna [tamaño, horizontal, x1, y1, x2, y2, x3, y3, x4, y4]
// int* colocar_barco(bool horizontal, int barco, int x, int y)
// {
//     int* coordenadas = (int*)malloc(2 + (barco * 2 * sizeof(int)));
//     coordenadas[0] = barco;
//     coordenadas[1] = horizontal;
//     if (horizontal)
//     {
//         for (int i = x; i < x + barco; i++)
//         {
//             jugador[y][i] = 1;
//             coordenadas[2 + (i - x) * 2] = 1;
//         }
//     }
//     else
//     {
//         for (int i = y; i < y + barco; i++)
//         {
//             jugador[i][x] = 1;
//             coordenadas[2 + (i - y) * 2] = 1;
//         }
//     }
//     return coordenadas;
// }
void colocar_barco(bool horizontal, int barco, int x, int y)
{
    if (horizontal)
    {
        for (int i = x; i < x + barco; i++)
        {
            jugador[y][i] = 1;
        }
    }
    else
    {
        for (int i = y; i < y + barco; i++)
        {
            jugador[i][x] = 1;
        }
    }
}

void pedir_coordenadas(int barco)
{
    printf("  Escoge la orientación de tu barco de largo %d: \n", barco);
    printf("    - '1' para horizontal\n");
    printf("    - '2' para vertical\n");
    char orientacion;
    scanf(" %c", &orientacion);
    // Mientras no se ingrese una orientación válida (1 o 2 o sea un char)
    while (strcmp(&orientacion, "1") != 0 && strcmp(&orientacion, "2") != 0)
    {
        printf("Orinetación no válida, ingrese nuevamente: ");
        printf("  Ingrese una orientación válida: \n");
        scanf(" %c", &orientacion);
    }
    printf("  Escoge la coordenada x: \n");
    scanf(" %c", &coordenada_x);
    // mientras no sea una letra en [A-E, a-e]
    while (coordenada_x != 'A' && coordenada_x != 'a' && coordenada_x != 'B' && coordenada_x != 'b' && coordenada_x != 'C' && coordenada_x != 'c' && coordenada_x != 'D' && coordenada_x != 'd' && coordenada_x != 'E' && coordenada_x != 'e')
    {
        printf("Coordenada no válida, intenta de nuevo: \n");
        printf("  Escoge la coordenada x: \n");
        scanf(" %c", &coordenada_x);
    }
    if (coordenada_x == 'A' || coordenada_x == 'a') x = 0;
    if (coordenada_x == 'B' || coordenada_x == 'b') x = 1;
    if (coordenada_x == 'C' || coordenada_x == 'c') x = 2;
    if (coordenada_x == 'D' || coordenada_x == 'd') x = 3;
    if (coordenada_x == 'E' || coordenada_x == 'e') x = 4;
    printf("  Escoge la coordenada y: \n");
    scanf(" %c", &coordenada_y);
    while ((strcmp(&coordenada_y, "1") != 0) &&
           (strcmp(&coordenada_y, "2") != 0) && 
           (strcmp(&coordenada_y, "3") != 0) && 
           (strcmp(&coordenada_y, "4") != 0) && 
           (strcmp(&coordenada_y, "5") != 0))
    {
        printf("Coordenada no válida, ingrese nuevamente: \n");
        printf("  Escoge la coordenada y: \n");
        scanf("%d", &y);
    }
    // transformar coordenada y a int
    y = coordenada_y - '0';
    if (strcmp(&orientacion, "1") == 0) horizontal = true;
    else horizontal = false;
    y--;
    while (!validar_coordenadas(horizontal, barco, x, y))
    {
        printf("Coordenadas no válidas, escoge otra vez\n");
        printf("  Escoge la orientación de tu barco de largo %d: \n", barco);
        printf("    - '1' para horizontal\n");
        printf("    - '2' para vertical\n");
        scanf("%c", &orientacion);
        while (strcmp(&orientacion, "1") != 0 && strcmp(&orientacion, "2") != 0)
        {
            printf("Orientación no válida, intenta de nuevo: \n");
            printf("  Ingrese una orientación válida: \n");
            scanf(" %c", &orientacion);
        }
        printf("  Escoge la coordenada x: ");
        scanf(" %c", &coordenada_x);
        while (coordenada_x != 'A' && coordenada_x != 'a' && coordenada_x != 'B' && coordenada_x != 'b' && coordenada_x != 'C' && coordenada_x != 'c' && coordenada_x != 'D' && coordenada_x != 'd' && coordenada_x != 'E' && coordenada_x != 'e')
        {
            printf("Coordenada no válida, intenta de nuevo: \n");
            printf("  Escoge la coordenada x: \n");
            scanf(" %c", &coordenada_x);
        }
        if (coordenada_x == 'A' || coordenada_x == 'a') x = 0;
        if (coordenada_x == 'B' || coordenada_x == 'b') x = 1;
        if (coordenada_x == 'C' || coordenada_x == 'c') x = 2;
        if (coordenada_x == 'D' || coordenada_x == 'd') x = 3;
        if (coordenada_x == 'E' || coordenada_x == 'e') x = 4;
        printf("  Escoge la coordenada y: ");
        scanf(" %c", &coordenada_y);
        while ((strcmp(&coordenada_y, "1") != 0) &&
               (strcmp(&coordenada_y, "2") != 0) &&
               (strcmp(&coordenada_y, "3") != 0) && 
               (strcmp(&coordenada_y, "4") != 0) && 
               (strcmp(&coordenada_y, "5") != 0))
        {
            printf("Coordenada no válida, ingrese nuevamente: \n");
            printf("  Escoge la coordenada y: \n");
            scanf("%d", &y);
        }
        y = coordenada_y - '0';
        if (strcmp(&orientacion, "1") == 0) horizontal = true;
        else horizontal = false;
        y--;
    }
}

bool pedir_disparo()
{
    printf("  Elige las coordenadas donde disparar a tu oponente\n");
    printf("    Escoge la coordenada x: ");
    scanf(" %c", &coordenada_x);
    while (coordenada_x != 'A' && coordenada_x != 'a' &&
           coordenada_x != 'B' && coordenada_x != 'b' &&
           coordenada_x != 'C' && coordenada_x != 'c' && 
           coordenada_x != 'D' && coordenada_x != 'd' && 
           coordenada_x != 'E' && coordenada_x != 'e')
    {
        printf("Coordenada no válida, intenta de nuevo: \n");
        printf("  Escoge la coordenada x: \n");
        scanf(" %c", &coordenada_x);
    }
    if (coordenada_x == 'A' || coordenada_x == 'a') x = 0;
    if (coordenada_x == 'B' || coordenada_x == 'b') x = 1;
    if (coordenada_x == 'C' || coordenada_x == 'c') x = 2;
    if (coordenada_x == 'D' || coordenada_x == 'd') x = 3;
    if (coordenada_x == 'E' || coordenada_x == 'e') x = 4;
    printf("    Escoge la coordenada y: ");
    scanf(" %c", &coordenada_y);
    while ((strcmp(&coordenada_y, "1") != 0) &&
           (strcmp(&coordenada_y, "2") != 0) && 
           (strcmp(&coordenada_y, "3") != 0) && 
           (strcmp(&coordenada_y, "4") != 0) && 
           (strcmp(&coordenada_y, "5") != 0))
    {
        printf("Coordenada no válida, ingrese nuevamente: \n");
        printf("  Escoge la coordenada y: \n");
        scanf(" %c", &coordenada_y);
    }
    y = coordenada_y - '0';
    y--;
    if (oponente[y][x] == 0)
    {
        oponente[y][x] = 3;
        printf("\n\n\t¡Has fallado!\n");
        return false;
    }
    else if (oponente[y][x] == 1)
    {
        oponente[y][x] = 2;
        puntaje_jugador++;
        printf("\n\n\t¡Has acertado!\n");
        return true;
    }
    else if (oponente[y][x] == 2)
    {
        printf("\n\n\t¡Ya habías acertado en esa coordenada!\n");
        return false;
    }
    else if (oponente[y][x] == 3)
    {
        printf("\n\n\t¡Ya habías fallado en esa coordenada!\n");
        return false;
    }
    return false;
}

int main()
{
    iniciar_tablero();
    mostrar_tablero();
    for (int i = 4; i > 1; i--)
    {
        pedir_coordenadas(i);
        colocar_barco(horizontal, i, x, y);
        mostrar_tablero();
    }
    printf("\t ¡Listo! Ya tienes tus barcos colocados\n");
    printf("\t Ahora es el turno de tu oponente\n");
    printf("\t Presiona cualquier tecla para continuar\n");
    // getch();
    // system("cls");
    mostrar_tablero();
    while (puntaje_jugador < 10)
    {
        if (pedir_disparo()) mostrar_tablero();
        else mostrar_tablero();
    }
    printf("\t ¡Felicidades! Has ganado\n");
}
