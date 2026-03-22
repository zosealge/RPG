# RPG
Simple real-time roguelike RPG made using ncurses

- RPG game version 0.3.6
- Map Editor version 0.3.1

<img width="1004" height="695" alt="rpg_screenshot" src="https://github.com/user-attachments/assets/aa0abc0f-1d01-463f-8706-3089b69ddaaf" />

  
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
