#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define SIZE 10
#define NUM_B5 1
#define NUM_B4 1
#define NUM_B3 2
#define NUM_B2 1

// #define B5_SIZE 5
// #define B4_SIZE 4
// #define B3_SIZE 3
// #define B2_SIZE 2

// #define INIT_BARCOS (barco_S) \
//                     { \
//                     .b5 = 1, \
//                     .b4 = 1, \
//                     .b3 = 2, \
//                     .b2 = 1 \
//                     }

typedef enum barco_e
{
    BARCO0 = 0,
    BARCO2 = 2,
    BARCO3,
    BARCO4,
    BARCO5,
    BARCOTOT        // 6
} barco_E;

typedef struct coord_s 
{
    uint8_t x;
    uint8_t y;
} coord_S;

typedef struct barco_s
{
    coord_S b5[BARCO5];
    coord_S b4[BARCO4];
    coord_S b3_0[BARCO3];
    coord_S b3_1[BARCO3];
    coord_S b2[BARCO2];
} barco_S;

typedef struct celda_s {
    char state;
    barco_E barco;
} celda_S;

typedef struct datos_s 
{
    uint8_t numero : 2;
    uint8_t barcosDerrotados : 3;
    uint8_t turno : 1;
} datos_S;
typedef struct jugador_s
{
    celda_S tablero[SIZE][SIZE];
    char juego[SIZE][SIZE];
    barco_S barcos;
    datos_S datos;
} jugador_S;

void printTablero(jugador_S*);
void printJuego(jugador_S*);
void fillBoard(jugador_S *j, barco_E b);
void enterCoordX(uint8_t*);
void enterCoordY(uint8_t*);
// _Bool checkCell(jugador_S*, uint8_t, uint8_t);
void turno(jugador_S*, jugador_S*);
void decideNextTurn(jugador_S*, jugador_S*);

int main(void)
{
    jugador_S j1;
    jugador_S j2;

    memset(&j1, 0, sizeof(j1));
    memset(&j2, 0, sizeof(j2));

    j1.datos.numero = 1;
    j1.datos.turno = (uint8_t)true;
    j2.datos.numero = 2;

    printf("Bienvenido Jugador %d: \n", j1.datos.numero);
    fillBoard(&j1, BARCO5);
    fillBoard(&j1, BARCO4);
    fillBoard(&j1, BARCO3);
    fillBoard(&j1, BARCO3);
    fillBoard(&j1, BARCO2);
    // printTablero(&j1);

    system("clear");

    printf("Bienvenido Jugador %d: \n", j2.datos.numero);
    fillBoard(&j2, BARCO5);
    fillBoard(&j2, BARCO4);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO2);
    printTablero(&j2);

    system("clear");

    // START GAME
    // while (j1.datos.barcosDerrotados != 5)
    // {
    //     if (j1.datos.turno == 1)
    //     {
    //         /* code */
    //     }
        
        
        
        
        
        
        
    //     decideNextTurn(&j1, &j2);
    // }
    
    // printJuego(&j1);
    // printJuego(&j2);

    return 0;
}

void decideNextTurn(jugador_S *j1, jugador_S *j2)
{

}

void printTablero(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d -> ", jugador->datos.numero);
    printf("Tablero de Barcos. \n");
    printf("\n");
    for (size_t i = 0; i < SIZE; i++)
    {
        printf("|");
        for (size_t j = 0; j < SIZE; j++)
        {
            char lt = (jugador->tablero[i][j].state == 0) ? ' ' : jugador->tablero[i][j].state;
            printf("%c", lt);
            printf("|");
        }
        printf("\n");
    }
}

void printJuego(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d -> ", jugador->datos.numero);
    printf("Tablero de Juego.\n");
    for (size_t i = 0; i < SIZE; i++)
    {
        printf("|");
        for (size_t j = 0; j < SIZE; j++)
        {
            char lt = (jugador->juego[i][j] == 0) ? ' ' : jugador->juego[i][j];
            printf("%c", lt);
            printf("|");
        }
        printf("\n");
    }
}

void enterCoordX(uint8_t *coordx)
{
    printf("x: ");
    scanf("%i", coordx);
}

void enterCoordY(uint8_t *coordy)
{
    printf("y: ");
    scanf("%i", coordy);
}

void fillBoard(jugador_S *j, barco_E b)
{
    static const char letraBarco[BARCOTOT] = {'0', 'E', 'D', 'C', 'B', 'A'};
    uint16_t i = 0;
    uint32_t x = 0; // Problem with memory if uint8_t is used
    uint32_t y = 0;
    unsigned char dir[2];
    _Bool valid = true;
    _Bool repeat = false;

    printf("Ingresa las coordernadas iniciales para el barco %d: \n", b);
    enterCoordX(&x);
    enterCoordY(&y);

    uint32_t x_i = x;
    uint32_t y_i = y;

    printf("Eje x o eje y: ");
    scanf("%1s", &dir);
    fflush(stdin);

    while (i < (uint16_t)b)
    {
        if ( (j->tablero[x][y].barco != BARCO0) || (x > 9) || (y > 9) || \
                ((dir[0] == 'x') && ((y_i + (uint16_t)b - 1) > 9)) || \
                    ((dir[0] == 'y') && ((x_i + (uint16_t)b - 1) > 9)) )
        {
            valid = false;
            printf("1 o más coordenadas, ocupadas, erroneas o fuera de rango. Ingrese de nuevo.\n");
            repeat = true;
            break;
        }

        if (dir[0] == 'x') {
            y++;
        }
        else if (dir[0] == 'y') {
            x++;
        }
        else {
            // No action
        }
        
        ++i;
    }
    i = 0;
    while ((i < (uint16_t)b) && (valid) && (!repeat))
    {
        j->tablero[x_i][y_i].state = letraBarco[(uint16_t)b];
        j->tablero[x_i][y_i].barco = b;
        switch (b)
        {
            case BARCO2:
                j->barcos.b2[i].x = x_i;
                j->barcos.b2[i].y = y_i;
                break;
            case BARCO3:
                if (j->barcos.b3_0[i].x == 0)
                {
                    j->barcos.b3_0[i].x = x_i;
                }
                else
                {
                    j->barcos.b3_1[i].x = x_i;
                }

                if (j->barcos.b3_0[i].y == 0)
                {
                    j->barcos.b3_0[i].y = y_i;
                }
                else
                {
                    j->barcos.b3_1[i].y = y_i;
                }
                break;
            case BARCO4:
                j->barcos.b4[i].x = x_i;
                j->barcos.b4[i].y = y_i;
                break;
            case BARCO5:
                j->barcos.b5[i].x = x_i;
                j->barcos.b5[i].y = y_i;
                break;
            default:
            case BARCO0:
                break;
        }
        y_i += (dir[0] == 'x') ? 1 : 0;
        x_i += (dir[0] == 'y') ? 1 : 0;
        ++i;
    }

    if (repeat)
    {
        repeat = false;
        fillBoard(j, b);
    }
    else
    {
        printTablero(j);
        printf("\n");
    }
}

void turno(jugador_S* j_a, jugador_S* j_d)
{
    uint32_t x = 0; // Problem with memory if uint8_t is used
    uint32_t y = 0;
    _Bool valid = true;

    printf("Ingresa la coordernada de ataque: \n");
    enterCoordX(&x);
    enterCoordY(&y);

    if ((x > 9) || (y > 9))
    {
        valid = false;
        printf("Coordenada fuera de rango. Ingrese de nuevo.\n");
        turno(j_a, j_d);
    }
    else if (j_a->juego[x][y] == 'X') {
        valid = false;
        printf("Coordenada ingresada previamente. Ingrese de nuevo.\n");
        turno(j_a, j_d);
    }
    else {
        j_a->juego[x][y] = 'X';
    }
    

    if (j_d->tablero[x][y].barco != BARCO0)
    {
        // Le diste a uno
        j_d->tablero[x][y].state = 'X';
        j_d->tablero[x][y].barco = BARCO0;
        // Revisa si ya acabaste con un barco
        // Continua tirando.
    }
    
    
}

// void checkBoardStatus(jugador_S*)
// {
//     uint8_t b5 = 5;
//     uint8_t b4 = 4;
//     uint8_t b3 = 6;
//     uint8_t b2 = 2;
//     for (size_t i = 0; i < SIZE; i++)
//     {
//         for (size_t j = 0; j < SIZE; j++)
//         {
            
//         }
//     }
// }