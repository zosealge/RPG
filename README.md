# RPG
Simple ASCII RPG Game with enemies, fireballs and... adventure
  
# COMPILE
make has -Wall -Wextra -Werror flags set
"make clean" for cleanup after compilation

written in linux

for macOS change the strncpy for strcpy_s

windows untested... and fk windows anyway

please use at least 100x30 terminal size

# TO DO
- map editor
- loading enemies start position from map header
- terminal size detect and prevent from start when too small
- move malloc from load submenu to program initialization section and pass pointer to load submenu

# KNOWN BUGS
- not every collision test are in place
- fireballs can fly infinite when out ouf bounds of window (or cause undefined behavior)
