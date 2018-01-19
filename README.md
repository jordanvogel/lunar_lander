# lunar_lander
Jordan Vogel

The improvements put into the lander are these:

    - Added an intro screen, it was a huge pain in the ass I hope you enjoy it
      
    - improved lander model, looks a bit more spacey!
      
    - smooth edge wrapping. If the ship exists the sides of the sketchpad, it will be
	teleported to the other side without losing momentum
      
    - fuel guage: with the "-i" flag in the call of the program, a gauge
      is added that starts at 1400 and decreases by 7 with every thrust key entry
      a red fuel guage is displayed at the top left shows how much fuel is left
      when the gauge is empty, you can no longer thrust


How to run:
    - "make" will build the lander

    - "make clean" will remove the built files of the lander

    Format to run: 
        ./lander -t (INT) -g (INT) -f landscape.txt 

        t is the thrust speed of the lander
        g is the gravity strength
        f is the text file that draws the map, if you want you can experiment with different maps
            its just sketchpad coordinates, and it draws lines between the points

        OPTIONAL:
            you can run it with -i to have a fuel gauge and an opening screen, also enables edge wrapping

        NOTE:
            when you run it you need to reselect the terminal for the inputs, if the sketchpad is selected
                it will make you sad

CONTROLS:
    Spacebar is to thrust
    Left arrow and right arrrow change the angle of the lander
    q is to quit and can be pressed at any time.
