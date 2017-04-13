/*
name: Jordan Vogel
StudentID: 1373051
*/

#include "functions.h"
#include "memwatch.h"

extern FILE * sketchpad;

//draws an object with given valeus
void drawObject (struct Object *object)
{
    for (int i = 0; i < object->arraySize; i++)
    {
        fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
	    lround(object->x1[i]), lround(object->y1[i]),
            lround(object->x2[i]), lround(object->y2[i])
            );
    }
}

//draws the lander
void drawLander (struct Object *lander)
{
    double print_x1;
    double print_y1;
    double print_x2;
    double print_y2;
    double a = lander->angle;
    double c[2] = {lander->center[0],lander->center[1]};
    
    for(int i = 0; i < (lander->arraySize); i++)
    {
        print_x1 = lander->x1[i] * cos(a) - lander->y1[i] * sin(a) + c[0];
        print_y1 = lander->x1[i] * sin(a) + lander->y1[i] * cos(a) + c[1];
        print_x2 = lander->x2[i] * cos(a) - lander->y2[i] * sin(a) + c[0];
        print_y2 = lander->x2[i] * sin(a) + lander->y2[i] * cos(a) + c[1];
        fprintf(sketchpad, "drawSegment %ld %ld %ld %ld\n",
	    lround(print_x1), lround(print_y1),
            lround(print_x2), lround(print_y2)
            );
    }
}

void eraseLander (struct Object *lander)
{
    double print_x1;
    double print_y1;
    double print_x2;
    double print_y2;
    double a = lander->angle;
    double c[2] = {lander->center[0],lander->center[1]};
    
    for(int i = 0; i < (lander->arraySize); i++)
    {
        print_x1 = lander->x1[i] * cos(a) - lander->y1[i] * sin(a) + c[0];
        print_y1 = lander->x1[i] * sin(a) + lander->y1[i] * cos(a) + c[1];
        print_x2 = lander->x2[i] * cos(a) - lander->y2[i] * sin(a) + c[0];
        print_y2 = lander->x2[i] * sin(a) + lander->y2[i] * cos(a) + c[1];

        fprintf(sketchpad, "eraseSegment %ld %ld %ld %ld\n",
	    lround(print_x1), lround(print_y1),
            lround(print_x2), lround(print_y2));
    }
}

//clears the sketchpad
void clearScreen ()
{
    fprintf(sketchpad, "clearScreen\n");
}

//ends the sketchpad
void sketchEnd ()
{
    fprintf(sketchpad, "end\n");
}

//initialize a given object
void initObject (struct Object *object, int x1list[], int y1list[],
            int x2list[], int y2list[], int size, double center[2], double angle)
{
    object->arraySize = size;
    object->x1 = malloc(object->arraySize*sizeof(double));
    object->y1 = malloc(object->arraySize*sizeof(double));
    object->x2 = malloc(object->arraySize*sizeof(double));
    object->y2 = malloc(object->arraySize*sizeof(double));
    for (int i = 0; i < object->arraySize; i++)
    {
        object->x1[i] = x1list[i];
        object->y1[i] = y1list[i];
        object->x2[i] = x2list[i];
        object->y2[i] = y2list[i];
    }
    object->center[0] = center[0];
    object->center[1] = center[1];
    object->angle = angle;
}

void freeMemory(struct Object *object)
{
    free(object->x1);
    free(object->y1);
    free(object->x2);
    free(object->y2);
}

void updateLanderObject(struct Object *lander, double updatedY, double updatedX)
{
    lander->center[1] = updatedY;
    lander->center[0] = updatedX;
}

void setLanderThrust(struct Object *lander)
{
    lander->arraySize=31;
}

void setLanderNoThrust(struct Object *lander)
{
    lander->arraySize=22;
}

void checkWrapping(struct Object *lander)
{
    if (lander->center[0]>639){
        lander->center[0] = 0;
    }
    if (lander->center[0]<0){
        lander->center[0] = 639;
    }
}

//most of this code is copied from alienryderflex.com/intersect
//for the handling and checking of line collision
int checkCollision(struct Object *lander, struct Object *landscape, int *crashedSeg)
{
    double Ax, Ay, Bx, By, Cx, Cy, Dx, Dy;
    
    
    for (int i = 0; i < 22; i++){
        for (int j = 0; j < landscape->arraySize; j++){
            Ax = lander->x1[i] * cos(lander->angle) - lander->y1[i] * sin(lander->angle) + lander->center[0];
            Ay = lander->x1[i] * sin(lander->angle) + lander->y1[i] * cos(lander->angle) + lander->center[1];
            Bx = lander->x2[i] * cos(lander->angle) - lander->y2[i] * sin(lander->angle) + lander->center[0];
            By = lander->x2[i] * sin(lander->angle) + lander->y2[i] * cos(lander->angle) + lander->center[1];
            Cx = landscape->x1[j];
            Cy = landscape->y1[j];
            Dx = landscape->x2[j];
            Dy = landscape->y2[j];
            double  distAB, theCos, theSin, newX, ABpos;
            
            //  Fail if either line segment is zero-length.
            if ((Ax==Bx && Ay==By) || (Cx==Dx && Cy==Dy)){
                continue;
            }
    
            //  Fail if the segments share an end-point.
            if ((Ax==Cx && Ay==Cy) || (Bx==Cx && By==Cy)
            ||  (Ax==Dx && Ay==Dy) || (Bx==Dx && By==Dy)) {
                continue; }
    
            //  (1) Translate the system so that point A is on the origin.
            Bx-=Ax; By-=Ay;
            Cx-=Ax; Cy-=Ay;
            Dx-=Ax; Dy-=Ay;
    
            //  Discover the length of segment A-B.
            distAB=sqrt(Bx*Bx+By*By);
    
            //  (2) Rotate the system so that point B is on the positive X axis.
            theCos=Bx/distAB;
            theSin=By/distAB;
            newX=Cx*theCos+Cy*theSin;
            Cy  =Cy*theCos-Cx*theSin; Cx=newX;
            newX=Dx*theCos+Dy*theSin;
            Dy  =Dy*theCos-Dx*theSin; Dx=newX;
    
            //  Fail if segment C-D doesn't cross line A-B.
            if ((Cy<0. && Dy<0.) || (Cy>=0. && Dy>=0.)){
                continue;
            }
    
            //  (3) Discover the position of the intersection point along line A-B.
            ABpos=Dx+(Cx-Dx)*Dy/(Dy-Cy);
    
            //  Fail if segment C-D crosses line A-B outside of segment A-B.
            if (ABpos<0. || ABpos>distAB){
                continue;
            }
    
            //  Success.

            *crashedSeg = j;
            return 1;
        }
    }
    return 0;
}

//ncurses functions from "test_curses.c" file
void setup_ncurses()
{
  initscr();
  cbreak();
  noecho();
  //nonl();
  curs_set(0);
  leaveok(stdscr, TRUE);
  //intrflush(stdscr, FALSE);

  keypad(stdscr, TRUE);
  nodelay(stdscr, TRUE);
}

void unset_ncurses()
{
  keypad(stdscr, FALSE);
  nodelay(stdscr, FALSE);
  nocbreak();
  echo();
  endwin();
}
