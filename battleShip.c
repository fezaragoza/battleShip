// battleShip.c -- by Fernando Zaragoza

/*
* INCLUDES
*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

/*
* DEFINES
*/
#define SIZE 10
#define TOTAL_BARCOS 5

/*
* TYPEDEFS
*/
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
    coord_S b3[2][BARCO3];
    coord_S b2[BARCO2];
} barco_S;

typedef struct barcoEstatus_s
{
    uint8_t b5;
    uint8_t b4;
    uint8_t b3[2];
    uint8_t b2;
} barcoEstatus_S;

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
    barcoEstatus_S bEstatus;
    datos_S datos;
} jugador_S;

/*
* FUNCTION PROTOTYPE DECLARATION
*/
void printHeader(void);
void printTablero(jugador_S*);
void printJuego(jugador_S*);
void enterCoordX(uint8_t*);
void enterCoordY(uint8_t*);
void fillBoard(jugador_S *j, barco_E b);
void decideNextTurn(jugador_S*, jugador_S*, _Bool*);
void turno(jugador_S*, jugador_S*);
void checkShipCell(jugador_S*, jugador_S*, uint8_t, uint8_t);

/*
* MAIN
*/
int main(void)
{
    jugador_S j1;
    jugador_S j2;
    _Bool game = true;

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

    system("clear");

    printf("Bienvenido Jugador %d: \n", j2.datos.numero);
    fillBoard(&j2, BARCO5);
    fillBoard(&j2, BARCO4);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO3);
    fillBoard(&j2, BARCO2);
    
    system("clear");

    // START GAME
    printf("A jugar. \n");
    while (game)
    {
        if (j1.datos.turno == 1)
        {
            printf("Turno de jugador %d.\n", j1.datos.numero);
            printJuego(&j1);
            turno(&j1, &j2); // J1 ataca, J2 defiende
        }
        else
        {
            printf("Turno de jugador %d.\n", j2.datos.numero);
            printJuego(&j2);
            turno(&j2, &j1); // J2 ataca, J1 defiende
        }
        decideNextTurn(&j1, &j2, &game);
    }
    printf("\n");
    return 0;
}


/*
* FUNCTION DEFINITIONS
*/
void printHeader(void)
{
    printf("   ");
    for (size_t i = 0; i < SIZE; i++)
    {
        printf("%i", i);
        printf(" ");
    }
    printf("\n");
}

void printTablero(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d -> ", jugador->datos.numero);
    printf("Tablero de Barcos. \n");
    printf("\n");
    printHeader();

    for (size_t i = 0; i < SIZE; i++)
    {
        printf("%d", i);
        printf(" |");
        for (size_t j = 0; j < SIZE; j++)
        {
            char lt = (jugador->tablero[i][j].state == 0) ? ' ' : jugador->tablero[i][j].state;
            printf("%c", lt);
            printf("|");
        }
        printf("\n");
    }
    printf("\n");
}

void printJuego(jugador_S *jugador)
{
    printf("\n");
    printf("Jugador %d -> ", jugador->datos.numero);
    printf("Tablero de Juego.\n");
    printf("\n");
    printHeader();

    for (size_t i = 0; i < SIZE; i++)
    {
        printf("%d", i);
        printf(" |");
        for (size_t j = 0; j < SIZE; j++)
        {
            char lt = (jugador->juego[i][j] == 0) ? ' ' : jugador->juego[i][j];
            printf("%c", lt);
            printf("|");
        }
        printf("\n");
    }
    printf("\n");
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
                if (j->barcos.b3[0][i].x == 0)
                {
                    j->barcos.b3[0][i].x = x_i;
                }
                else
                {
                    j->barcos.b3[1][i].x = x_i;
                }

                if (j->barcos.b3[0][i].y == 0)
                {
                    j->barcos.b3[0][i].y = y_i;
                }
                else
                {
                    j->barcos.b3[1][i].y = y_i;
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
        printf("Press ENTER key to Continue.\n");
        while(getchar()!='\n');
        getchar();
        // system("clear"); 
    }
}

void decideNextTurn(jugador_S *j1, jugador_S *j2, _Bool *game)
{
    if ((j1->datos.barcosDerrotados == TOTAL_BARCOS))
    {
        *game = false;
        printf("GANASTE JUGADOR: %d", j1->datos.numero);
    }
    else if ((j2->datos.barcosDerrotados == TOTAL_BARCOS))
    {
        *game = false;
        printf("GANASTE JUGADOR: %d", j2->datos.numero);
    }
    else if (j1->datos.turno == 1)
    {
        j1->datos.turno = 0;
        j2->datos.turno = 1;
    }
    else
    {
        j1->datos.turno = 1;
        j2->datos.turno = 0;
    }
}

void turno(jugador_S* j_a, jugador_S* j_d)
{
    uint32_t x = 0; // Problem with memory if uint8_t is used
    uint32_t y = 0;
    _Bool valid = true;
    _Bool repeat = false;

    printf("Ingresa la coordernada de ataque: \n");
    enterCoordX(&x);
    enterCoordY(&y);
    // sleep(2);
    // system("clear");

    if ((x > 9) || (y > 9))
    {
        valid = false;
        printf("Coordenada fuera de rango. Ingrese de nuevo.\n");
        repeat = true;
    }
    else if ((j_a->juego[x][y] == 'X') || (j_a->juego[x][y] == 'O')) {
        valid = false;
        printf("Coordenada ingresada previamente. Ingrese de nuevo.\n");
        repeat = true;
    }
    else {
        j_a->juego[x][y] = 'X';
    }
    

    if ((j_d->tablero[x][y].barco != BARCO0) && (valid)) // Le diste a uno
    {
        checkShipCell(j_a, j_d, x, y);
        repeat = true;
        printf("\n");
        printf("Le diste, repite turno.\n");
        printJuego(j_a);
        printf("\n");
    }
    else if (valid)
    {
        printf("\n");
        printf("No acertaste.\n");
    }
    else
    {
        // No action
    }

    // Continua tirando. repeat
    if (repeat)
    {
        if (j_a->datos.barcosDerrotados == TOTAL_BARCOS)
        {
            repeat = false;
        }
        else
        {
            repeat = false;
            turno(j_a, j_d);
        }
    }
    else
    {
        printJuego(j_a);
        // printf("\n");
        printf("Press ENTER key to Continue.\n");
        while(getchar()!='\n');
        getchar();
        system("clear"); 
    }
    
}

void checkShipCell(jugador_S* j_a, jugador_S* j_d, uint8_t x, uint8_t y)
{
    j_d->tablero[x][y].state = 'X';
        // Checar tablero para ver cual fue, y ver si ya se destruyó.
    switch (j_d->tablero[x][y].barco)
    {
        case BARCO2:
            for (size_t i = 0; i < BARCO2; i++)
            {
                if ((j_d->barcos.b2[i].x == x) && (j_d->barcos.b2[i].y == y))
                {
                    j_d->barcos.b2[i].x = 10;
                    j_d->barcos.b2[i].y = 10;

                    ++j_a->bEstatus.b2;
                    j_a->juego[x][y] = 'O'; // Cambia caracter para indicar que ahi habia un barco.
                }
                // Revisa si ya acabaste con el barco
                if (j_a->bEstatus.b2 == BARCO2)
                {
                    ++j_a->datos.barcosDerrotados;
                }
            }
            break;
        case BARCO3:
            for (size_t j = 0; j < 2; j++)
            {
                for (size_t i = 0; i < BARCO3; i++)
                {
                    if ((j_d->barcos.b3[j][i].x == x) && (j_d->barcos.b3[j][i].y == y))
                    {
                        j_d->barcos.b3[j][i].x = 10;
                        j_d->barcos.b3[j][i].y = 10;

                        ++j_a->bEstatus.b3[j];
                        j_a->juego[x][y] = 'O'; // Cambia caracter para indicar que ahi habia un barco.
                    }
                    // Revisa si ya acabaste con el barco
                    if (j_a->bEstatus.b3[j] == BARCO3)
                    {
                        j_a->bEstatus.b3[j] = 0;
                        ++j_a->datos.barcosDerrotados;
                    }
                }
            }
            
            break;
        case BARCO4:
            for (size_t i = 0; i < BARCO4; i++)
            {
                if ((j_d->barcos.b4[i].x == x) && (j_d->barcos.b4[i].y == y))
                {
                    j_d->barcos.b4[i].x = 10;
                    j_d->barcos.b4[i].y = 10;

                    ++j_a->bEstatus.b4;
                    j_a->juego[x][y] = 'O'; // Cambia caracter para indicar que ahi habia un barco.
                }
                // Revisa si ya acabaste con el barco
                if (j_a->bEstatus.b4 == BARCO4)
                {
                    ++j_a->datos.barcosDerrotados;
                }
            }
            break;
        case BARCO5:
            for (size_t i = 0; i < BARCO5; i++)
            {
                if ((j_d->barcos.b5[i].x == x) && (j_d->barcos.b5[i].y == y))
                {
                    j_d->barcos.b5[i].x = 10;
                    j_d->barcos.b5[i].y = 10;

                    ++j_a->bEstatus.b5;
                    j_a->juego[x][y] = 'O'; // Cambia caracter para indicar que ahi habia un barco.
                }
                // Revisa si ya acabaste con el barco
                if (j_a->bEstatus.b5 == BARCO5)
                {
                    ++j_a->datos.barcosDerrotados;
                }
            }
            break;
        default:
        case BARCO0:
            break;
    }

    j_d->tablero[x][y].barco = BARCO0; // Clear 
}