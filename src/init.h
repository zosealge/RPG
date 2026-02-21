void initrpgcolor(void)
{
    init_pair(1,  COLOR_RED,     COLOR_BLACK);      // ROAD r
    init_pair(2,  COLOR_MAGENTA, COLOR_GREEN);      // BUSH #
    init_pair(3,  COLOR_GREEN,   COLOR_GREEN);      // GRASS g
    init_pair(4,  COLOR_BLUE,    COLOR_BLACK);      // PLAYER @
    init_pair(5,  COLOR_MAGENTA, COLOR_MAGENTA);    // STONE s (soild wall)
    init_pair(6,  COLOR_BLUE,    COLOR_BLUE);       // WATER w
    init_pair(7,  COLOR_YELLOW,  COLOR_RED);        // DIRT d (destructable wall)
    init_pair(8,  COLOR_RED,     COLOR_BLACK);      // MANA (* - x) particle     
    init_pair(9,  COLOR_GREEN,   COLOR_BLACK);      // TREE T
    init_pair(10, COLOR_BLUE,    COLOR_GREEN);      // on grass - player/enemy backgrounds
    init_pair(11, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(12, COLOR_RED,     COLOR_BLACK);
    init_pair(13, COLOR_GREEN,   COLOR_BLACK);
    init_pair(14, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(15, COLOR_BLUE,    COLOR_BLACK);
    init_pair(16, COLOR_YELLOW,  COLOR_BLACK);
    init_pair(17, COLOR_RED,     COLOR_BLACK);
    init_pair(18, COLOR_GREEN,   COLOR_BLACK);
    init_pair(19, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(20, COLOR_BLUE,    COLOR_BLACK);
}