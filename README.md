# RPG
Simple ASCII RPG Game

v0.2:
- init version
- please use at least 100x30 terminal size

# COMPILE

- compile with -lncurses flag to link ncurses library
gcc or clang.

written in 64_bit linux

# CONTROLS

- use arrow keys to control Your ASCII symbol '@'
- use space bar to shoot fireballs

- menu can be invoked with 1
- instant game exit with 0
- kill all enemies on the screen 9 (will be removed)

# .MAP FILES

- uses three digits for every variable
- header contain player start position (enemy start position will be in the future)
- maps can be only 100x100 (possible more in the future)
- map data is YYY XXX ASCII_CHAR - 001 001 035

# TODO
- map editor
- proper map header (reading player start position and enemies start positions)
