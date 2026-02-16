void player_boom_anim(WINDOW *screen,int y, int x)
{
    const long WAIT_BOOM=96666666;

    wattron(screen,COLOR_PAIR(12));
    mvwaddstr(screen,y-1,x-1," * ");
    mvwaddstr(screen,y,x-1,  "*X*");
    mvwaddstr(screen,y+1,x-1," * ");
    wattroff(screen,COLOR_PAIR(12));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);

    wattron(screen,COLOR_PAIR(16));
    mvwaddstr(screen,y-1,x-1,"*X* ");
    mvwaddstr(screen,y,x-1,  "X X");
    mvwaddstr(screen,y+1,x-1,"*X*");
    wattroff(screen,COLOR_PAIR(16));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);

    wattron(screen,COLOR_PAIR(17));
    mvwaddstr(screen,y-1,x-1,"xxx");
    mvwaddstr(screen,y,x-1,  "x#x");
    mvwaddstr(screen,y+1,x-1,"xxx");
    wattroff(screen,COLOR_PAIR(17));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);

    wattron(screen,COLOR_PAIR(20));
    mvwaddstr(screen,y-1,x-1," . ");
    mvwaddstr(screen,y,x-1,  ".x.");
    mvwaddstr(screen,y+1,x-1," . ");
    wattroff(screen,COLOR_PAIR(20));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);

    wattron(screen,COLOR_PAIR(4));
    mvwaddch(screen,y,x,'_');
    wattroff(screen,COLOR_PAIR(4));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);
}