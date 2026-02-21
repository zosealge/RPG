# RPG
Simple ASCII RPG Game with enemies, fireballs and... adventure
  
# COMPILE
make rpg - to compile RPG
make editor - to compile map editor

written in linux

for macOS change the strncpy for strcpy_s

windows untested... and fk windows anyway

please use at least 100x30 terminal size

# TO DO
- terminal size detect and prevent from start when too small
- move malloc from load submenu to program initialization section and pass pointer to load submenu

# KNOWN BUGS
- entering into map selection submenu from entry screen, not selecting map, and exit cause seg fault
- enemies near walls dissapear for a moment
- shooting walls at zero distance erases them
- fireballs can fly infinite when out ouf bounds of window (or cause undefined behavior)
