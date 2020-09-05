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

#define INIT_BARCOS (barco_S) \
                    { \
                    .b5 = 1, \
                    .b4 = 1, \
                    .b3 = 2, \
                    .b2 = 1 \
                    }

typedef enum barco_e
{
    BARCO0 = 0,
    BARCO2 = 2,
    BARCO3,
    BARCO4,
    BARCO5,
    BARCOTOT        // 6
} barco_E;

typedef struct barco_s
{
    uint8_t b5 : 1;
    uint8_t b4 : 1;
    uint8_t b3 : 2;
    uint8_t b2 : 1;
} barco_S;

typedef struct celda_s {
    char state;
    barco_E barco;
} celda_S;

typedef struct jugador_s
{
    celda_S tablero[SIZE][SIZE];
    char juego[SIZE][SIZE];
    barco_S estadoBarcos;
    uint8_t numero;
} jugador_S;

void printTablero(jugador_S*);
void printJuego(jugador_S*);
void fillBoard(jugador_S *j, barco_E b);
void enterCoordX(uint8_t*);
void enterCoordY(uint8_t*);
// _Bool checkCell(jugador_S*, uint8_t, uint8_t);
void turno(jugador_S*, jugador_S*);

int main(void)
{
    jugador_S j1;
    jugador_S j2;

    memset(j1.tablero, 0, sizeof(j1.tablero));
    memset(j2.tablero, 0, sizeof(j2.tablero));
    memset(j1.juego, 0, sizeof(j1.juego));
    memset(j2.juego, 0, sizeof(j2.juego));

    j1.estadoBarcos = INIT_BARCOS;
    j2.estadoBarcos = INIT_BARCOS;

    j1.numero = 1;
    j2.numero = 2;

    printf("Bienvenido Jugador %d: \n", j1.numero);

    fillBoard(&j1, BARCO5);
    fillBoard(&j1, BARCO4);
    fillBoard(&j1, BARCO3);
    fillBoard(&j1, BARCO3);
    fillBoard(&j1, BARCO2);
    printTablero(&j1);

    printf("Bienvenido Jugador %d: \n", j2.numero);

    fillBoard(&j2, BARCO5);
    fillBoard(&j2, BARCO4);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO2);
    printTablero(&j2);

    // printJuego(&j1);
    // printJuego(&j2);

    return 0;
}

void printTablero(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d \n", jugador->numero);
    for (size_t i = 0; i < SIZE; i++)
    {
        for (size_t j = 0; j < SIZE; j++)
        {
            printf("\t");
            printf("%c", jugador->tablero[i][j].state);
            printf("\t");
            printf("|");
        }
        printf("\n");
        for (size_t k = 0; k < SIZE; k++)
        {
            printf("----------------");
        }
        printf("\n");
    }
}

void printJuego(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d \n", jugador->numero);
    for (size_t i = 0; i < SIZE; i++)
    {
        for (size_t j = 0; j < SIZE; j++)
        {
            // printf("\t");
            printf("%c", jugador->juego[i][j]);
            // printf("\t");
            printf("|");
        }
        printf("\n");
        for (size_t k = 0; k < SIZE; k++)
        {
            printf("--------");
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
                ((dir[0] == 'x') && ((y + (uint16_t)b) > 10)) || \
                    ((dir[0] == 'y') && ((x + (uint16_t)b) > 10)) )
        {
            valid = false;
            printf("1 o más coordenadas, ocupadas, erroneas o fuera de rango. Ingrese de nuevo.\n");
            fillBoard(j, b);
        }

        if (dir[0] == 'x') {
            y++;
        }
        else if (dir[0] == 'y') {
            x++;
        }
        else {

        }
        
        ++i;
    }
    i = 0;
    while ((i < (uint16_t)b) && (valid))
    {
        j->tablero[x_i][y_i].state = letraBarco[(uint16_t)b];
        j->tablero[x_i][y_i].barco = b;
        y_i += (dir[0] == 'x') ? 1 : 0;
        x_i += (dir[0] == 'y') ? 1 : 0;
        // if (dir[0] == 'x') {
        //     y_i++;
        // }
        // else if (dir[0] == 'y') {
        //     x_i++;
        // }
        ++i;
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