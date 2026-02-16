# RPG
Simple ASCII RPG Game with enemies, fireballs and... adventure
  
# COMPILE
compile with -lncurses flag to link ncurses library
gcc or clang

written in linux
probably will work on macos

please use at least 100x30 terminal size

# TO DO
- map editor
- loading enemies start position from map header
- terminal size detect and prevent from start when too small
- move malloc from load submenu to program initialization section and pass pointer to load submenu

# KNOWN BUGS
- after multiple map reloads, load submenu corrupts allowing to load empty string in place of file_name
- not every collision test are in place
- 30-40% CPU usage when in gameloop
