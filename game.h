/*
name: Jordan Vogel
StudentID: 1373051
*/

#define _POSIX_C_SOURCE 200112L
#define PI acos(-1.0)
#define FUEL 1400

#include <signal.h>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "functions.h"
#include "memwatch.h"
#include <ncurses.h>

void handle_timeout(int signal);
void updateLander();

int gameMain();
void drawLandscape();
void crashOrLand();
void exitProgram();
void drawFuel();
void eraseFuel();
void drawFuelOutline();
// Non-C99 compliant function prototypes (copied from the parse.h file)
FILE *popen(const char *command, const char *type);
int pclose(FILE *stream);


