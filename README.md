# RPG
Simple ASCII RPG Game

v0.2:
- init version
  
v0.3:
- basic gameloop and map read and map re-initialization works correctly
- added colorful animations
  
# COMPILE

- compile with -lncurses flag to link ncurses library
gcc or clang.

written in 64_bit linux
please use at least 100x30 terminal size

# CONTROLS

- use arrow keys to control Your ASCII symbol '@'
- use space bar to shoot fireballs

- menu can be invoked with 1
- instant game exit with 0
- (debug) instant zero healts 8
- (debug) kill all enemies on the screen 9

# .MAP FILES

- uses three digits for every variable
- header contain player start position (enemy start position will be in the future)
- maps can be only 100x100 (possible more in the future)
- map data is YYY XXX ASCII_CHAR - 001 001 035

# TODO
- map editor
- proper map header (reading player start position and enemies start positions)
