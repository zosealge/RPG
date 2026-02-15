# RPG
Simple ASCII RPG Game with enemies, fireballs and... adventure
  
# COMPILE
"maptest" is the current release, stripped from main menu just to work on map read and draw functions.

compile with -lncurses flag to link ncurses library
gcc or clang

written in linux
probably will work on macos

please use at least 100x30 terminal size

# TO DO
- main menu in separate header file
- map editor
- welcome screen with nice text animation
- loading enemies start position from map header

# KNOWN BUGS
- sometimes welcome screen needs a little more time to end
- map draw from memory leaves one empty spot on y26 x2
- dead enemies will not revive on map reload
