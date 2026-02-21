# RPG
Simple ASCII RPG Game with enemies, fireballs and... adventure

binaries are compiled on openSUSE Tumbleweed x86_64

RPG game version 0.3.5
Map Editor version 0.2.0
  
# COMPILE
- make rpg - to compile RPG
- make editor - to compile map editor

written in linux

for macOS change the strncpy for strcpy_s

windows untested... and fk windows anyway

please use at least 100x30 terminal size

# TO DO
- terminal size detect and prevent from start when too small
- move malloc from load submenu to program initialization section and pass pointer to load submenu

# KNOWN BUGS
- entering into map selection submenu from entry screen, not selecting map, and exit cause seg fault
- shooting walls at zero distance erases them
- enemies collision work in progress - can be buggy, enemies sometimes dissapear near walls
- fireballs can fly infinite when out ouf bounds of window (or cause undefined behavior)
