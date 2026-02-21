.PHONY : editor
editor:
	-gcc -o edit ./src/editor.c -Wall -Wextra -Werror -lncurses

.PHONY: rpg
rpg:
	-gcc -o rpg ./src/main.c -Wall -Wextra -Werror -lncurses