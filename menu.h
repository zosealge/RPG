int loadmenu(struct map_data *data0);

int newmenu(struct map_data *data0)
{
    //returns
    // 0 - new game
    // 1 - exit
    const int MENU_Y=4;
    const int MENU_X=15;
    bool startgame=false;
    bool menuloop=true;
    int ret_load;
    int y=1;
    int x=1;
    int ch;
    int ch_symb;
    char curs='>';
    WINDOW *m;
    m=newwin(MENU_Y,MENU_X,6,6);
    box(m,0,0);
    mvwaddstr(m,0,2,"---=RPG=---");
    mvwaddch(m,y,x,curs);
    mvwaddstr(m,1,2,"New Game");
    mvwaddstr(m,2,2,"Exit...");
    wrefresh(m);
    do
    {
        switch(ch=wgetch(m))
        {
            case 65:
            ch_symb=mvwinch(m,y-1,x);
            if(ch_symb==4194417) break;
            mvwaddch(m,y,x,' ');
            y--;
            mvwaddch(m,y,x,curs);
            break;
            case 66:
            ch_symb=mvwinch(m,y+1,x);
            if(ch_symb==4194417) break;
            mvwaddch(m,y,x,' ');
            y++;
            mvwaddch(m,y,x,curs);
            break;
            case 10:
            if(y==1)
            {
                ret_load=loadmenu(data0);
                //touchwin(m);
                //wrefresh(m);
                menuloop=false;
                startgame=true;
            }
            else
            {
                menuloop=false;
            }
            break;
        }
    }
    while(menuloop);
    delwin(m);
    if(startgame) return 0;
    return 1;
}
int mainmenu(bool gameloop,struct map_data *data0)
{
    // this function returns
    // 0 - continue game (or restart on death)
    // 1 - 1 load new map
    // 2 - exit game
    const int MENU_Y=5;
    const int MENU_PAUSE_X=15;
    bool menuloop=true;
    bool menuload=false;
    bool mapload=false;
    bool setreturnexit=false;
    int ret_load;
    int y=1;
    int x=1;
    int ch;
    int ch_symb;
    char curs='>';
    WINDOW *m;
    m=newwin(MENU_Y,MENU_PAUSE_X,6,6);
    box(m,0,0);                  
    mvwaddstr(m,0,2,"---=Menu=---");
    mvwaddch(m,y,x,curs);
    if(gameloop) mvwaddstr(m,1,2,"Continue");
    else mvwaddstr(m,1,2,"Restart");
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
                if(y==1) menuloop=false;
                else if(y==2)
                {
                    ret_load=loadmenu(data0);
                    touchwin(m);
                    wrefresh(m);
                    if(ret_load==0)
                    {
                        y=1;
                    }
                    else if(ret_load==1)
                    {
                        menuloop=false;
                        mapload=true;
                    }
                }
                else if(y==3)
                {
                    menuloop=false;
                    setreturnexit=true;
                }
            break;
            default:
            break;
        }
        wrefresh(m);
    }
    while(menuloop);
    delwin(m);
    if(setreturnexit) return 2;
    if(mapload) return 1;
    return 0;
}

int loadmenu(struct map_data *data0)
{
    //returns:
    // 0 - no load
    // 1 - load new map
    // 2 - error
    //
    const int MENU_LOAD_X=15;
    const char *map_folder="maps";
    char **menupos=malloc(sizeof(char*)*8*8);
    if(menupos==NULL) return 2;
    for(int i=0;i<8;i++)
    {
        menupos[i]=malloc(sizeof(char)*8*8);
        if(menupos[i]==NULL) return 2;
    }
    char curs='>';
    int list_y=1;
    int exit=-1;
    int ch;
    int ch_symb;
    int y=1;
    int x=1;
    int i_num=0;
    bool loadmenu=true;
    bool loadmap=false;
    struct dirent *map_folder_obj;
    WINDOW *m;
    DIR *mapdir;
    mapdir=opendir(map_folder);
    if(mapdir==NULL)
    {
        return 2;
    }
    while(map_folder_obj=readdir(mapdir))
    {
        strcpy(menupos[i_num],map_folder_obj->d_name);
        list_y++;
        exit++;
        i_num++;
    }
    closedir(mapdir);
    m=newwin(list_y,MENU_LOAD_X,7,7);
    box(m,0,0);
    mvwaddstr(m,0,2,"---=Load=---");
    for(int i=2;i<=i_num;i++)
    {
        mvwaddstr(m,i-1,2,menupos[i]);
    }
    mvwaddstr(m,exit,2,"Exit");
    mvwaddch(m,y,x,curs);
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
            case 10:
            if(y==exit)
            {
                loadmap=false;
                loadmenu=false;
            }
            else
            {
                //mvwaddstr(m,0,1,menupos[y+1]);
                strcpy(data0->file_name,menupos[y+1]);
                loadmap=true;
                loadmenu=false;
            }
            break;
            default:
            break;
        }
    }
    while(loadmenu);
    for(int i=0;i<8;i++) free(menupos[i]);
    free(menupos);
    if(loadmap)
    {

        delwin(m);
        return 1;
    }
    delwin(m);
    return 0;
}

void welcomescreen(void)
{
    const long WAIT_MENU=99999999;
    bool welcome_screen=true;
    int ch;
    int i=11;
    WINDOW *w;
    w=newwin(30,100,0,0);
    mvwaddstr(w,13,2,"v0.3.2");
    mvwaddstr(w,14,2,"Press any key to continue");
    mvwaddstr(w,17,2," shoot         move  ");
    mvwaddstr(w,18,2,"fireballs       with ");
    mvwaddstr(w,19,2," with          [^]   ");
    mvwaddstr(w,20,2,"[SPACE BAR] [<][V][>]");
    mvwaddstr(w,16,32,"         in game:");
    mvwaddstr(w,17,32,"open menu        [~]");
    mvwaddstr(w,18,32,"99 mana          [7]");
    mvwaddstr(w,19,32,"destroy player   [8]");
    mvwaddstr(w,20,32,"kill enemies     [9]");
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
