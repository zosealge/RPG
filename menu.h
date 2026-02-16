char *mainmenu(bool gameloop,struct map_data *data0)
{
    // this function returns char pointer
    // "cont"  - exit menu and continue game
    // "ex"    - exit gameloop, deinitialize memory and exit
    // mapname - only 8 chars - reload map
    bool menuloop=true;
    int y=1;
    int x=1;
    int ch;
    int ch_symb;
    char curs='>';
    WINDOW *m;
    m=newwin(10,30,6,6);
    box(m,0,0);
    mvwaddstr(m,0,2,"---=Menu=---");
    mvwaddch(m,y,x,curs);
    if(gameloop) mvwaddstr(m,1,2,"Continue");
    else mvwaddstr(m,1,2,"New Game");
    mvwaddstr(m,2,2,"Load Map");
    mvwaddstr(m,3,2,"Exit");
    wrefresh(m);
    do
    {
        switch(ch=wgetch(m))
        {
            case 65: //UP
            ch_symb=mvwinch(m,y-1,x);
            if(ch_symb==4194417) break;
            else
            {
                mvwaddch(m,y,x,' ');
                y--;
                mvwaddch(m,y,x,curs);
            }
            break;
            case 66: //DOWN
            ch_symb=mvwinch(m,y+1,x);
            if(ch_symb==4194417) break;
            else
            {
                mvwaddch(m,y,x,' ');
                y++;
                mvwaddch(m,y,x,curs);
            }
            break;
            case 10: //ENTER
            menuloop=false;
            break;
            default:
            break;
        }
        wrefresh(m);
    }
    while(menuloop);
    delwin(m);
}

void welcomescreen(void)
{
    const long WAIT_MENU=99999999;
    bool welcome_screen=true;
    int ch;
    int i=11;
    WINDOW *w;
    w=newwin(30,100,0,0);
    mvwaddstr(w,13,2,"Test version for debugging process");
    mvwaddstr(w,14,2,"Press any key to load :test.map:");
    mvwaddstr(w,16,2,"           in game:");
    mvwaddstr(w,17,2,"open menu        [~]");
    mvwaddstr(w,18,2,"test.map         [1]");
    mvwaddstr(w,19,2,"mordor.map       [2]");
    mvwaddstr(w,21,2,"99 mana          [7]");
    mvwaddstr(w,22,2,"destroy player   [8]");
    mvwaddstr(w,23,2,"kill enemies     [9]");
    nodelay(w,true);
    do
    {
        switch(ch=wgetch(w))
        {
            case 48: // 0 key
            case 27: // ESC key
            break;
            case 10: // ENTER key
            case 32: // SPACE key
                welcome_screen=false;
            break;
            default: //use for resize try
            break;
        }
            //ANIMATION YAAAY
        wattron(w,COLOR_PAIR(i));
        mvwaddstr(w,3, 2,"        ####                      x  ");
        wattroff(w,COLOR_PAIR(i));
        wattron(w,COLOR_PAIR(i+1));
        mvwaddstr(w,4, 2,"        #  ##                    x   ");
        wattroff(w,COLOR_PAIR(i+1));
        wattron(w,COLOR_PAIR(i+2));
        mvwaddstr(w,5, 2,"        ####   #####  ##        x    ");
        wattroff(w,COLOR_PAIR(i+2));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,6, 2,"        #  #    #  # #         x     ");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,7, 2,"        #  #    ###  #   #   dxb     ");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+4));
        mvwaddstr(w,8, 2,"        #  ###  #     ####   x       ");
        wattroff(w,COLOR_PAIR(i+4));
        if(welcome_screen)
        {
            i++;
            if(i>=17) i=11;
            wait_nano(WAIT_MENU);
        }

    }
    while(welcome_screen);
    //delwin(w);
}