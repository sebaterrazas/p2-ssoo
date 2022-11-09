#include <stdio.h>
#include <string.h>

int i,j,h,jugador[5][5], oponente[5][5], corYi = 0, corXi = 5, corYf = 0, corXf = 5, puntajeoponente, puntajejugador;
int barco_valido, corXd = 0, corYd = 0, fin_juego = 0;
char corXs1, corXsd;

void imprimir_tablero()
{
    printf("\t\t\n");
    printf("\t     BATALLA NAVAL \n");
    printf("\t     A  B  C  D  E\n");
    // Imprimimos el tablero del oponente
    for (i=0; i<5; i++)
    {
        printf("\t");
        for (j=0; j<5; j++)
        {            
            if (i == 0 && j == 0)
            {
                printf(" 1 ");
            }
            if (i == 1 && j == 0)
            {
                printf(" 2 ");
            }
            if (i == 2 && j == 0)
            {
                printf(" 3 ");
            }
            if (i == 3 && j == 0)
            {
                printf(" 4 ");
            }
            if (i == 4 && j == 0)
            {
                printf(" 5 ");
            }
            if (oponente[i][j] == 3)
            {
                printf("  -");
            }
            else
            {
                if (oponente[i][j] == 2)
                {
                    printf("  X");
                }
                else
                {
                    printf("  *");
                }
            }
        }
        if (i==3)
        {
            printf("\t\t\t OPONENTE");
        }
        else
        {
            if (i==1)
            {
                printf("\t\t\t Puntaje oponente: %d", puntajeoponente);
            }
        }
        printf("\n");
    }
    printf("\n\n\t __________________ \n\n");
    //Imprimimos tablero del oponente
    printf("\t     A  B  C  D  E\n");
    for (i = 0; i<5; i++)
    {
        printf("\t");
        for (j = 0; j<5; j++)
        {
            if (i == 0 && j == 0)
            {
                printf(" 1 ");
            }
            if (i == 1 && j == 0)
            {
                printf(" 2 ");
            }
            if (i == 2 && j == 0)
            {
                printf(" 3 ");
            }
            if (i == 3 && j == 0)
            {
                printf(" 4 ");
            }
            if (i == 4 && j == 0)
            {
                printf(" 5 ");
            }
            if (jugador[i][j] == 3)
            {
                printf("  -");
            }
            else
            {
                if (jugador[i][j] == 2)
                {
                    printf("  X");
                }
                else
                {
                    if (jugador[i][j] == 1)
                    {
                        printf("  O");
                    }
                    else
                    {
                        printf("  *");
                    }
                }
            }
        }
        if (i==3)
        {
            printf("\t\t\t JUGADOR");
        }
        else
        {
            if (i==1)
            {
                printf("\t\t\t Puntaje jugador: %d", puntajejugador);
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void iniciar_tablero()
{
    for (int i = 0 ; i < 5 ; i++)
    {
        for (int j = 0 ; j < 5 ; j++) 
        {
            jugador[i][j] = 0;
            oponente[i][j] = 0;
        }
    }
}

int validar_coordenadaX(char x)
{
    int validez = 0;
    if (strcmp(&x, "A") == 0 || strcmp(&x, "a") == 0 )
    {
        validez++;        
    }
    if (strcmp(&x, "B") == 0 || strcmp(&x, "b") == 0 )
    {
        validez++; 
    }
    if (strcmp(&x, "C") == 0 || strcmp(&x, "c") == 0 )
    {
        validez++; 
    }
    if (strcmp(&x, "D") == 0 || strcmp(&x, "d") == 0 )
    {
        validez++; 
    }
    if (strcmp(&x, "E") == 0 || strcmp(&x, "e") == 0 )
    {
        validez++; 
    }
    if (validez == 1)
    {
        return 0; //Coordenada valida
    }
    else
    {
        return 1; //Coordenada invalida
    }
}

int validar_coordenadaY(int y)
{
    if (y < 1 || y > 5)
    {
        return 1; //Coordenada invalida
    }

    else
    {
        return 0; //Coordenada valida
    }   
}

int transformar_coordenadaX(char x)
{
    int valor_final;
    if (strcmp(&x, "A") == 32 || strcmp(&x, "a") == 32 )
    {
        valor_final = 0;
    }
    if (strcmp(&x, "B") == 32 || strcmp(&x, "b") == 32 )
    {
        valor_final = 1;
    }
    if (strcmp(&x, "C") == 32 || strcmp(&x, "c") == 32 )
    {
        valor_final = 2;
    }
    if (strcmp(&x, "D") == 32 || strcmp(&x, "d") == 32 )
    {
        valor_final = 3;
    }
    if (strcmp(&x, "E") == 32 || strcmp(&x, "e") == 32 )
    {
        valor_final = 4;
    }
    return valor_final;
}

int transformar_coordenadaY(int y)
{
    return y-1;
}    

void poner_barco(int xi, int yi, int largo, int orientacion)
{
    if (orientacion == 0)// Es en orientacion vertical
    {
        for (i = 0; i < largo; i++)
        {
            jugador[yi + i][xi] = 1; 
        }
    }
    
    else // Es en orientación horizontal
    {
        for (i = 0; i < largo; i++)
        {
            jugador[yi][xi + i] = 1; 
        }
    }
}

int revisar_tablero(int x_i, int y_i, int x_f, int y_f, int largo_barco)
{
    int posicion_final_x1;
    int posicion_final_x2;
    int posicion_final_y1;
    int posicion_final_y2;
    int contador_espacios = 0;
    int orientacion; // 0 vertical | 1 horizontal
    if (jugador[y_i][x_i] == 0)//Punto inicial disponible
    {
        printf("Pos inicial vacia !!\n");
        contador_espacios++;
        if (x_i == x_f) //Revisamos columna
        {
            posicion_final_x1 = x_f;
            posicion_final_y1 = y_i + largo_barco - 1;
            posicion_final_y2 = y_i - largo_barco + 1;
            orientacion = 0;
            for (h = 1; h < largo_barco; h++)
            {
                if (y_i > y_f)
                {
                    if (jugador[y_f + h][x_f] == 0)
                    {
                        contador_espacios++;   
                    }
                }
                else
                {
                    if (jugador[y_i + h][x_i] == 0)
                    {
                        contador_espacios++;   
                    }
                }
            }
        }

        if (y_i == y_f) //Revisamos la fila
        {
            posicion_final_x1 = x_i + largo_barco - 1;
            posicion_final_x2 = x_i - largo_barco + 1;
            posicion_final_y1 = y_f;
            orientacion = 1;
            for (h = 1; h < largo_barco; h++)
            {
                if (x_i > x_f)
                {
                    if (jugador[y_f][x_f + h] == 0)
                    {
                        contador_espacios++; 
                    }
                }
                else
                {
                    if (jugador[y_i][x_i + h] == 0)
                    {
                        contador_espacios++; 
                    }
                }
            }
        }
    }

    if (contador_espacios == largo_barco)
    {
        if (posicion_final_x1 == x_f && posicion_final_y1 == y_f)
        {
            poner_barco(x_i,y_i,largo_barco,orientacion);
            imprimir_tablero();
            return 0;
        }  
        if (posicion_final_x1 == x_f && posicion_final_y2 == y_f)
        {
            poner_barco(x_f,y_f,largo_barco,orientacion);
            imprimir_tablero();
            return 0;
        }
        if (posicion_final_x2 == x_f && posicion_final_y1 == y_f)
        {
            poner_barco(x_f,y_f,largo_barco,orientacion);
            imprimir_tablero();
            return 0;
        }
    }

    else
    {
        if (contador_espacios != largo_barco)
        {
            printf("No hay suficiente espacio disponible para tu barco \n");
            return 1;
        }
        else
        {
            printf("Recuerda que el largo de tu barco es %d \n", largo_barco);
            return 1;
        }        
    }
}

void pedir_coordenadas(int largo_barco)
{
    printf("\t Escoge la posición de tu barco de largo %d: \n",largo_barco);
    //Coordenada Inicial
    printf("Inicio: \n");
    printf("Coordenada X: ");
    scanf(" %c", &corXs1);

    int val_x = validar_coordenadaX(corXs1); 

    while (val_x == 1)
    {
        printf("\tCoordenada X invalida\n");
        printf("Coordenada X: ");
        scanf(" %c", &corXs1);
        val_x = validar_coordenadaX(corXs1); 
    }

    printf("Coordenada Y: ");
    scanf("%i", &corYi);

    int val_y = validar_coordenadaY(corYi);

    while (val_y == 1)
    {
        printf("\tCoordenada Y invalida\n");
        printf("Coordenada Y: ");
        scanf(" %i", &corYi);
        val_y = validar_coordenadaY(corYi); 
    } 

    corYi = transformar_coordenadaY(corYi);
    corXi = transformar_coordenadaX(corXs1);

    //Coordenada Final
    printf("Fin: \n");
    printf("Coordenada X: ");
    scanf(" %c", &corXs1);

    val_x = validar_coordenadaX(corXs1); 

    while (val_x == 1)
    {
        printf("\tCoordenada X invalida\n");
        printf("Coordenada X: ");
        scanf(" %c", &corXs1);
        val_x = validar_coordenadaX(corXs1); 
    }

    printf("Coordenada Y: ");
    scanf("%i", &corYf);

    val_y = validar_coordenadaY(corYf);

    while (val_y == 1)
    {
        printf("\tCoordenada Y invalida\n");
        printf("Coordenada Y: ");
        scanf(" %i", &corYf);
        val_y = validar_coordenadaY(corYf); 
    } 

    corYf = transformar_coordenadaY(corYf);
    corXf = transformar_coordenadaX(corXs1);
}

void pedir_disparo()
{
    printf("\t Elige las coordenadas para disparar a tu oponente \n");
    printf("Coordenada X: ");
    scanf(" %c", &corXsd);

    int val_x = validar_coordenadaX(corXsd); 

    while (val_x == 1)
    {
        printf("\tCoordenada X invalida\n");
        printf("Coordenada X: ");
        scanf(" %c", &corXsd);
        val_x = validar_coordenadaX(corXsd); 
    }

    printf("Coordenada Y: ");
    scanf("%i", &corYd);

    int val_y = validar_coordenadaY(corYd);

    while (val_y == 1)
    {
        printf("\tCoordenada Y invalida\n");
        printf("Coordenada Y: ");
        scanf(" %i", &corYd);
        val_y = validar_coordenadaY(corYd); 
    } 

    corYd = transformar_coordenadaY(corYd);
    corXd = transformar_coordenadaX(corXsd);
}

void resultado_disparo(int x, int y)
{    
    if (oponente[y][x] == 2) // Había una parte de un barco ya encontrada anteriormente
    {
        printf("Esta posición ya fue atacada, ya le diste a este blanco! \n");
        oponente[y][x] = 2; // Sigue como barco golpeado
    }
    if (oponente[y][x] == 1) // Había una parte de un barco no encontrada hasta ahora
    {
        printf("Felicidades, le diste al blanco! \n");
        oponente[y][x] = 2; // Cambia a barco golpeado
        puntajejugador++; // Agregar +1 a "puntajejugador"
    }
    if (oponente[y][x] == 3) // Había un misil fallido, ya se había lanzado allí sin éxito
    {
        printf("Esta posición ya fue atacada, no habia nada aquí :( \n");
        oponente[y][x] = 3; // Sigue como misil fallido
    }
    if (oponente[y][x] == 0) // No había nada en esa ubicación
    {
        printf("Lo sentimos, no habia nada en esta posición :( \n");
        oponente[y][x] = 3; // Cambia a misil fallido
    }
    imprimir_tablero();
}

int main ()
{
    iniciar_tablero();

    //Esto se puso para poder probar los disparos
    oponente[0][0] = 1;
    oponente[2][0] = 1;
    oponente[3][0] = 1;
    oponente[4][0] = 1;
    oponente[0][1] = 1;
    oponente[0][2] = 1;
    oponente[0][3] = 1;
    oponente[0][4] = 1;
    oponente[1][1] = 1;
    //______________________________________________//

    imprimir_tablero();
    //Jugador posiciona sus barcos
    //--------------------  BARCO LARGO 4 --------------------//
    pedir_coordenadas(4);
    barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,4);

    while(barco_valido == 1) //No se pudo poner el barco
    {
        pedir_coordenadas(4);
        barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,4);
    }

    //--------------------  BARCO LARGO 3 --------------------//
    pedir_coordenadas(3);
    barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,3);

    while(barco_valido == 1) //No se pudo poner el barco
    {
        pedir_coordenadas(3);
        barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,3);
    }

    //--------------------  BARCO LARGO 2 --------------------//
    pedir_coordenadas(2);
    barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,2);

    while(barco_valido == 1) //No se pudo poner el barco
    {
        pedir_coordenadas(2);
        barco_valido = revisar_tablero(corXi,corYi,corXf,corYf,2);
    }

    //Comienza la partida
    while(fin_juego != 1)
    {
        pedir_disparo();
        resultado_disparo(corXd,corYd);
        if (puntajejugador == 9) //Gano el jugador
        {
            fin_juego = 1; 
            printf("FELICITACIONES, Has ganado !!!\n");
        }
        if (puntajeoponente == 9) // Gano el oponente
        {
            fin_juego = 1;
            printf("Lo siento, has perdido :(\n");
        }
    }   
}