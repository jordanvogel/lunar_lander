/*
name: Jordan Vogel
StudentID: 1373051
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memwatch.h"
#include "game.h"
#include "intro.h"


double gravity;
int thrust;
char filename[128];
int improve;

char *sketchname = "java -jar Sketchpad.jar";    
FILE * sketchpad;

int main(int argc, char* argv[]) 
{      
    gravity = 0;
    thrust = 0;
    filename[0] = 0;
    improve = 0;
    //handle the command args
    int i;
    for(i = 1; i<argc; i++)
    {
        if (strcmp("-g", argv[i]) == 0)
        {
            gravity = atof(argv[i+1]);
        }
        if (strcmp("-t", argv[i]) == 0)
        {
            thrust = atoi(argv[i+1]);
        }
        if (strcmp("-f", argv[i]) == 0)
        {
            strcpy(filename, argv[i+1]);
        }
        if (strcmp("-i", argv[i]) == 0)
        {
            improve = 1;
        }
        
    }
    //check for landscape
    if(strcmp(filename, "")==0)
    {
        printf("No landscape file specified\nUse -f option for landscape file\n");
        exit(EXIT_SUCCESS);
    }
    
    FILE *testFile = fopen(filename, "r");    
    //check if the file is valid
    if (!(testFile)){
        printf("Error: %s doesn't exist.\n", filename);
        return 0;
    }
    
    
    if (thrust > 0){
        thrust = thrust*(-1);
    }
    
    sketchpad = popen(sketchname, "w");
    printf(" ");
    
    if (improve)
    {
    introMain();
    }

    //PLEASE WORK GAME PLEASE!!!!!
    gameMain();
    
    exitProgram();
}
