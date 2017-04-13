/*
name: Jordan Vogel
StudentID: 1373051
*/

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <ncurses.h>
#include "memwatch.h"

#ifndef functions_H
#define functions_H
#define _XOPEN_SOURCE 700

//struct for objects
struct Object{
    double* x1;
    double* y1;
    double* x2;
    double* y2;
    int arraySize;
    double center[2];
    double angle;
};

#endif


//func prototypes
void drawObject (struct Object *object);
void drawLander (struct Object *lander);
void eraseLander (struct Object *lander);
void clearScreen ();
void sketchEnd ();
void initObject (struct Object *object, int x1list[], int y1list[],
            int x2list[], int y2list[], int size, double center[2], double angle);
void freeMemory(struct Object *object);

void updateLanderObject(struct Object *lander, double updatedY, double updatedX);
void setLanderThrust(struct Object *lander);
void setLanderNoThrust(struct Object *lander);
void checkWrapping(struct Object *lander);
int checkCollision(struct Object *lander, struct Object *landscape, int *crashedSeg);
void setup_ncurses();
void unset_ncurses();
