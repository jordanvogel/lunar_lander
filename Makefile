# Jordan Vogel
# 1373051

# Creates lander exec file, clean will remove all created files 

#CFLAGS = -g

lander: main.o functions.o game.o memwatch.o intro.o
	gcc $(CFLAGS) -Wall -std=c99 -o lander main.o functions.o intro.o game.o memwatch.o -lm -lcurses

intro.o: intro.c intro.h functions.h
	gcc $(CFLAGS) -Wall -std=c99 -c -DMEMWATCH -DMW_STDIO intro.c

main.o: main.c memwatch.h
	gcc $(CFLAGS) -Wall -std=c99 -c -DMEMWATCH -DMW_STDIO main.c 

functions.o: functions.c functions.h game.h
	gcc $(CFLAGS) -Wall -std=c99 -c -DMEMWATCH -DMW_STDIO functions.c

game.o: game.c game.h
	gcc $(CFLAGS) -Wall -std=c99 -c -DMEMWATCH -DMW_STDIO game.c

memwatch.o: memwatch.c memwatch.h
	gcc $(CFLAGS) -std=c99 -Wall -c -DMEMWATCH -DMW_STDIO memwatch.c 


clean:
	-rm -f *.o main core
	-rm -f *.o intro core
	-rm -f *.o functions core
	-rm -f *.o game core
	-rm -f *.o *.log memwatch core 

tar:
	tar cvf final7778.tar main.c intro.c intro.h game.c game.h functions.c\
 functions.h memwatch.c memwatch.h README Makefile
