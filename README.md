# RPG
Simple real-time roguelike RPG made using ncurses

- RPG game version 0.3.6
- Map Editor version 0.3.1
  
# COMPILE
- make rpg - to compile RPG
- make editor - to compile map editor

written in linux, will compile on MacOS too

ncurses with developer library needed for compilation

windows untested... and fk windows anyway

please use at least 100x30 terminal size - any terminal will do

# TO DO
- terminal size detect and prevent from start when too small
- end game when all enemies are dead
- bigger maps with scrolling
- inventory system

# KNOWN BUGS
- entering into map selection submenu from entry screen, not selecting map, and exit cause seg fault
- shooting walls at zero distance erases them
- enemies collision work in progress - can be buggy, enemies sometimes dissapear near walls
- fireballs can fly infinite when out ouf bounds of window (or cause undefined behavior)
