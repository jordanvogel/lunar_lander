/*
Jordan Vogel
StudentID: 1373051
*/

#include "game.h"
#include "memwatch.h"

sigset_t block_mask_g;

extern double gravity;
extern int thrust;
extern char filename[128];
extern int improve;
extern FILE * sketchpad;

double Yvel;
double Xvel;
double updatedY;
double updatedX;
//thrust value is set from command args entered by user
double thrustToggle = 0;
//angleSwitch is used to determine how to rotate the ship based on key press
int angleSwitch = 0;
int c;
int crashedSeg;
double fuel;
double fuelPct;

struct Object lander;
struct Object landscapeObj;

int gameMain()
{
    //draw the ground and improvements
    fuel = FUEL;
    drawLandscape();
    if(improve)
        drawFuelOutline();
    Yvel = 0;
    Xvel = 0;
    setup_ncurses();
    fuelPct = round((fuel/FUEL)*70);
    
    //draws the ship
    int x1list[] = {-5,-3,3,5,5,-3,-5,-5,-3,3,-6,4,-4,-3,3,-3,-6,6,1,-7,-2,5,-3,-4,4,-5,5,-4,4,-3,3};
    int y1list[] = {-7,-9,-9,-7,-3,1,-3,-7,2,2,3,3,4,4,4,6,3,3,3,8,8,8,6,7,7,9,9,13,13,15,15};
    int x2list[] = {-3,3,5,5,3,3,-3,-5,-3,4,-4,6,4,-3,3,3,-6,6,1,-5,2,7,3,-4,4,-5,5,-4,4,1,1};
    int y2list[] = {-9,-9,-7,-3,1,1,1,-3,2,2,3,3,4,6,6,6,7,7,8,8,8,8,6,8,7,12,12,14,14,17,17};
    double center[2] = {300,200};
    double angle = (0*PI/180.0);
    //initalize spaceship at position 300,200 and upright
    initObject(&lander, x1list, y1list, x2list, y2list, 31, center, angle);
    setLanderNoThrust(&lander);
    
    //code used from timer.c
    sigemptyset(&block_mask_g); 
    sigaddset(&block_mask_g, SIGALRM); 
    struct sigaction handler;
    handler.sa_handler = handle_timeout;
    sigemptyset(&handler.sa_mask);
    handler.sa_flags = 0;
    if (sigaction(SIGALRM, &handler, NULL)  == -1)
        exit(EXIT_FAILURE);
    struct itimerval timer;
    struct timeval time_delay;
    time_delay.tv_sec = 0;
    time_delay.tv_usec = 50000;
    timer.it_interval = time_delay;
    struct timeval start;
    start.tv_sec = 0;
    start.tv_usec = 50000;
    timer.it_value = start;

    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
        exit(EXIT_FAILURE);

    for ( ; ; )
        ;
    sketchEnd();
    exit(EXIT_SUCCESS);
}


//SIGALRM handling
void handle_timeout(int signal)
{
    if (signal == SIGALRM)
    {
    struct itimerval timer;
    if (getitimer(ITIMER_REAL, &timer)  == -1)
        exit(EXIT_FAILURE);
    //handle user inputs
    c = wgetch(stdscr);    
    if (c != ERR)
    {
        if (c == KEY_LEFT)
        {
            angleSwitch = 1;  
        } else if (c == KEY_RIGHT)
        {
            angleSwitch = 2;
        } else if (c == ' ')
        {
            if (fuel > 0)
            {
            thrustToggle = thrust;
            if(improve)
                fuel = fuel - 7;
            setLanderThrust(&lander);
            }
        } 
        else if (c == 'q')
        {
            timer.it_interval.tv_sec = 0;
            timer.it_value.tv_sec = 0;
            timer.it_interval.tv_usec = 0;
            timer.it_value.tv_usec = 0;
            exitProgram();
        } 
    }
    erase();
    refresh();
    
    //update position and speed of the lander
    updatedX = lander.center[0] + (Xvel * 0.05) + ((1/2) * (-thrustToggle*sin(lander.angle)) * 0.05*0.05);
    Xvel = Xvel + (-thrustToggle*sin(lander.angle)) * 0.05; 

    updatedY = lander.center[1] + (Yvel * 0.05) + ((1/2) * (gravity+thrustToggle*cos(lander.angle)) * 0.05*0.05);
    Yvel = Yvel + (gravity+thrustToggle*cos(lander.angle)) * 0.05;
        
    updateLander();
    if (improve)
    {
        checkWrapping(&lander);
    }
    else if (lander.center[0]> 639)
    {
        nocbreak();
        printf("Lander stuck press 'q' to quit\n");
        int c =  getch();
        while (1)
        {
            if (c != ERR)
            {
                if (c == 'q')
                {
                    exitProgram();
                    break;
                } 
            }
            c = getch();
        }
    }
    else if (lander.center[0]< 0)
    {
        nocbreak();
        printf("Lander stuck press 'q' to quit\n");
        int c =  getch();
        while (1)
        {
            if (c != ERR)
            {
                if (c == 'q')
                {
                    exitProgram();
                    break;
                } 
            }
            c = getch();
        }
    }
    

    //check for collision
    if(checkCollision(&lander, &landscapeObj, &crashedSeg))
    {
        crashOrLand();
    }
    if (setitimer(ITIMER_REAL, &timer, NULL) == -1)
        exit(EXIT_FAILURE);
  }

}

void updateLander(){
    sigset_t old_mask;
    if (sigprocmask(SIG_BLOCK, &block_mask_g, &old_mask) == -1)
        exit(EXIT_FAILURE);
    //if -i is called update the fuel gauge  
    if(improve)
    {    
        eraseFuel();
        drawFuel();
    }
    
    // rotate the lander based on inputs
    if (angleSwitch == 1)
    {
        lander.angle = ((lander.angle*180/PI)-15)*PI/180;
        angleSwitch = 0;
    } else if (angleSwitch == 2)
    {
        lander.angle = ((lander.angle*180/PI)+15)*PI/180;
        angleSwitch = 0;
    }
    if (round(lander.angle*180/PI) == -360 || round(lander.angle*180/PI) == 360)
    {
        lander.angle = 0;
    }
    thrustToggle = 0;
    updateLanderObject(&lander, updatedY, updatedX);
    drawLander(&lander);
    fflush(sketchpad);
    eraseLander(&lander);
    setLanderNoThrust(&lander);
        
    if (sigprocmask(SIG_SETMASK, &old_mask, NULL) == -1)
        exit(EXIT_FAILURE);
}

void drawLandscape()
{
    FILE *landscapeFile = fopen(filename, "r");

    int pointCount = 0;
    char line[10];
    int xArray[20];
    int yArray[20];
    int x1Array[20];
    int y1Array[20];
    int x2Array[20];
    int y2Array[20];
    
    while (fgets(line, 10, landscapeFile))
    {
        sscanf(line, "%d%d", &xArray[pointCount], &yArray[pointCount]);
        pointCount++;
    }
    
    for (int i=0; i<(pointCount-1); i++)
    {
        x1Array[i] = xArray[i];
        y1Array[i] = yArray[i];
        x2Array[i] = xArray[i+1];
        y2Array[i] = yArray[i+1];        
    }
    double center[2] = {0,0};
    initObject(&landscapeObj, x1Array, y1Array, x2Array, y2Array, pointCount-1, center, 0);
    drawObject(&landscapeObj);
    fflush(sketchpad);
    fclose(landscapeFile);
}

void crashOrLand()
{
    nocbreak();
    int crash = 0;
    int j = crashedSeg;
    //check slope of the segment the ship crashed in
    if (((landscapeObj.y2[j] - landscapeObj.y1[j])/(landscapeObj.x2[j] - landscapeObj.x1[j]))!=0)
    {
        crash = 1;
    }
    //make sure ship is upright
    if (round(lander.angle*180/PI) != 0)
    {
        crash = 1;
    }
    //check speed
    if (Yvel> 22)
    {
        crash = 1;
    }
    
    if(crash == 1){
        printf("You crashed the lander ");
    } 
    else 
    {
        printf("You landed the lander ");
    }
    printf("at a speed of %.2lf !!    Press 'q' to quit\n ", Yvel);
    //printf("at a speed of %.2lf !!", Yvel);
    int c =  getch();
    while (1)
    {
        if (c != ERR)
        {
            if (c == 'q')
            {
                exitProgram();
            } 
        }
        c = getch();
    }
    
}

void drawFuelOutline()
{
    fprintf(sketchpad, "drawSegment 20 20 91 20\n");
    fprintf(sketchpad, "drawSegment 91 20 91 32\n");
    fprintf(sketchpad, "drawSegment 20 20 20 32\n");
    fprintf(sketchpad, "drawSegment 20 32 91 32\n");
}

void eraseFuel()
{
    for (int i=0; i<fuelPct; i++)
    {
        fprintf(sketchpad, "eraseSegment %d 21 %d 31\n", 21+i, 21+i);
    }
}
void drawFuel()
{
    fuelPct = round((fuel/FUEL)*70);
    fprintf(sketchpad, "setColor 255 0 0\n");
    for (int i=0; i<fuelPct; i++)
    {
        fprintf(sketchpad, "drawSegment %d 21 %d 31\n", 21+i, 21+i);
    }
    fprintf(sketchpad, "setColor 0 0 0\n");
}

void exitProgram()
{
    unset_ncurses();
    freeMemory(&lander);
    freeMemory(&landscapeObj);
    clearScreen();
    sketchEnd();
    pclose(sketchpad);
    exit(EXIT_SUCCESS);
}
