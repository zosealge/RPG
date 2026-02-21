/*

loadmenu()
savemenu()
yousure()

are used inside menu.h

functions:

newmenu()
mainmenu()
editmenu()
welcomescreen()
editorscreen()
showhelp()

*/

int loadmenu(struct map_data *data0);
int savemenu(struct map_data *data0);
int yousure(void);

int newmenu(struct map_data *data0)
{
    //returns
    // 0 - new game
    // 1 - exit
    const int MENU_Y=4;
    const int MENU_X=15;
    bool startgame=false;
    bool menuloop=true;
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
                loadmenu(data0);
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

int editmenu(struct map_data *data0)
{
    // this function returns
    // 0 - continue editor
    // 1 - 1 load new map
    // 2 - exit game
    // 3 - save map
    const int MENU_Y=6;
    const int MENU_PAUSE_X=15;
    bool menuloop=true;
    bool mapload=false;
    bool mapsave=false;
    bool setreturnexit=false;
    int ret_load;
    int ret_save;
    int y=1;
    int x=1;
    int ch;
    int ch_symb;
    char curs='>';
    WINDOW *m;
    m=newwin(MENU_Y,MENU_PAUSE_X,6,6);
    box(m,0,0);                  
    mvwaddstr(m,0,2,"--=Editor=--");
    mvwaddch(m,y,x,curs);
    mvwaddstr(m,1,2,"Continue");
    mvwaddstr(m,2,2,"Save Map");
    mvwaddstr(m,3,2,"Load Map");
    mvwaddstr(m,4,2,"Exit");
    wmove(m,y,x);
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
                else if(y==2) // save
                {
                    ret_save=savemenu(data0);
                    touchwin(m);
                    wrefresh(m);
                    if(ret_save==0)
                    {
                        y=2;
                    }
                    else if(ret_save==1)
                    {
                        mapsave=true;
                        menuloop=false;
                    }
                }
                else if(y==3) // load
                {
                    ret_load=loadmenu(data0);
                    touchwin(m);
                    wrefresh(m);
                    if(ret_load==0)
                    {
                        y=3;
                    }
                    else if(ret_load==1)
                    {
                        menuloop=false;
                        mapload=true;
                    }
                }
                else if(y==4) // exit
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
    if(mapsave) return 3;
    if(setreturnexit) return 2;
    if(mapload) return 1;
    return 0;
}

int savemenu(struct map_data *data0)
{
    const char *map_ext=".map";
    bool saveloop=true;
    bool savefile=false;
    char buffer[13];
    int ch;
    int x=2;
    int cnt=0;
    WINDOW *s;
    s=newwin(5,30,5,4);
    box(s,0,0);
    mvwaddstr(s,0,2,"---=Save=---");
    mvwaddstr(s,1,1,"Name: [ENTER] or [~] cancel");
    mvwaddch(s,2,1,'>');
    wmove(s,2,x);
    wrefresh(s);
    do
    {
        switch(ch=wgetch(s))
        {
            case 10:
            if(mvwinch(s,2,2)==95)
            {
                mvwaddstr(s,3,1,"Provide name!");
            }
            else
            {
                if(yousure()==1)
                {
                    touchwin(s);
                    wmove(s,2,2);
                    winnstr(s,buffer,cnt);
                    strncat(buffer,map_ext,4);
                    strncpy(data0->file_name,buffer,12);
                    savefile=true;
                }
                saveloop=false;
            }
            break;
            case 96:
                saveloop=false;
            break;
            default:
            if(ch<=47 && ch>=123)
            {
                mvwaddstr(s,3,1,"No special symbols!");
            }
            else
            {
                if(x>=10)
                {
                    x=2;
                    cnt=1;
                }
                mvwaddstr(s,3,1,"                   ");
                mvwaddch(s,2,x,ch);
                x++;
                cnt++;
            }
            break;
        }
        wrefresh(s);
    }
    while(saveloop);
    delwin(s);
    if(savefile)
    {
        return 1;
    }
    return 0;
}

int loadmenu(struct map_data *data0)
{
    //returns:
    // 0 - no load
    // 1 - load new map
    // 2 - error
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
    while((map_folder_obj=readdir(mapdir)))
    {
        strncpy(menupos[i_num],map_folder_obj->d_name,sizeof(char)*13);
        list_y++;
        exit++;
        i_num++;
    }
    closedir(mapdir);
    m=newwin(list_y,MENU_LOAD_X,7,7);
    box(m,0,0);
    mvwaddstr(m,0,2,"---=Load=---");
    for(int i=2;i<i_num;i++)
    {
        mvwaddnstr(m,i-1,2,menupos[i],12);
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
                strncpy(data0->file_name,menupos[y+1],sizeof(char)*13);
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
    mvwaddstr(w,13,2,"v0.3.5");
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
        mvwaddstr(w,3, 2,"       ## RRRRRR   PPPPPPPP   GGGGGG  ##");
        wattroff(w,COLOR_PAIR(i));
        wattron(w,COLOR_PAIR(i+1));
        mvwaddstr(w,4, 2,"     ####  RR  RR    PP   PP GG    GG ####");
        wattroff(w,COLOR_PAIR(i+1));
        wattron(w,COLOR_PAIR(i+2));
        mvwaddstr(w,5, 2," <=######  RRRRR     PPPPPP  GG       ######");
        wattroff(w,COLOR_PAIR(i+2));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,6, 2,"   ######  RR  RR    PP      GG  GGGG ######=>");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,7, 2,"     ####  RR   RR   PP      GG    GG ####");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+4));
        mvwaddstr(w,8, 2,"       ## RRRR   RR PPPP      GGGGGG  ##");
        wattroff(w,COLOR_PAIR(i+4));
        if(welcome_screen)
        {
            i++;
            if(i>=17) i=11;
            wait_nano(WAIT_MENU);
        }

    }
    while(welcome_screen);
}

void editorscreen(void)
{
    const long WAIT_MENU=99999999;
    bool editor_screen=true;
    int ch;
    int i=11;
    WINDOW *w;
    w=newwin(30,100,0,0);
    mvwaddstr(w,13,2,"v0.2.0");
    mvwaddstr(w,14,2,"Press any key to continue");
    mvwaddstr(w,17,2," erase        move  ");
    mvwaddstr(w,18,2,"  stuff        with ");
    mvwaddstr(w,19,2," with          [^]   ");
    mvwaddstr(w,20,2,"[BACKSPACE] [<][V][>]");
    mvwaddstr(w,16,32,"         in game:");
    mvwaddstr(w,17,32,"open menu        [~]");
    mvwaddstr(w,18,32,"display stuff    [1]");
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
                editor_screen=false;
            break;
            default: //use for resize try
            break;
        }
            //ANIMATION YAAAY

        wattron(w,COLOR_PAIR(i));
        mvwaddstr(w,3, 2,"    EEEE   DDD   I TT TTTT  OOOOO    RRR");
        wattroff(w,COLOR_PAIR(i));
        wattron(w,COLOR_PAIR(i+1));
        mvwaddstr(w,4, 2,"   E      D   D      T     O     O  R   R");
        wattroff(w,COLOR_PAIR(i+1));
        wattron(w,COLOR_PAIR(i+2));
        mvwaddstr(w,5, 2,"   EEE    D    D I   T     O     O   RRR");
        wattroff(w,COLOR_PAIR(i+2));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,6, 2,"   E      D    D I   T     O     O  R   R");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+3));
        mvwaddstr(w,7, 2,"   E      D   D  I   T     O     O  R    R");
        wattroff(w,COLOR_PAIR(i+3));
        wattron(w,COLOR_PAIR(i+4));
        mvwaddstr(w,8, 2,"    EEEEE  DDD   I   T      OOOOO   R     R");
        wattroff(w,COLOR_PAIR(i+4));
        if(editor_screen)
        {
            i++;
            if(i>=17) i=11;
            wait_nano(WAIT_MENU);
        }

    }
    while(editor_screen);
}

void showhelp(void)
{
        /*
        r 370
        # 547
        g 871
        @ 1088
        s 1395
        w 1655
        d 1892
        * 2090
        T 2388
        */
    WINDOW *h;
    bool showhelp=true;
    int ch;
    curs_set(0);
    h=newwin(20,35,5,4);
    box(h,0,0);
    mvwaddstr(h,0,2,"---=Help=---");
    wrefresh(h);
     mvwaddch(h,1,1,547);
    mvwaddstr(h,1,3,"- [Z] wooden box");

     mvwaddch(h,2,1,1395);
    mvwaddstr(h,2,3,"- [X] stone wall");

     mvwaddch(h,3,1,2090);
    mvwaddstr(h,3,3,"- [C] mana orb");

     mvwaddch(h,4,1,2388);
    mvwaddstr(h,4,3,"- [V] tree");

     mvwaddch(h,5,1,1088);
    mvwaddstr(h,5,3,"- [Q] set player start position");

     mvwaddch(h,6,1,37);
    mvwaddstr(h,6,3,"- [W] set 1st enemy position");

     mvwaddch(h,7,1,36);
    mvwaddstr(h,7,3,"- [E] set 2nd enemy position");

     mvwaddch(h,8,1,38);
    mvwaddstr(h,8,3,"- [R] set 3rd enemy position");

     mvwaddch(h,9,1,33);
    mvwaddstr(h,9,3,"- [T] set 4th enemy position");

    mvwaddstr(h,11,1,"[SPACE] Empty");
    mvwaddstr(h,12,1,"[BACKSPACE] Erase");
    mvwaddstr(h,14,1,"Press key in bracket");
    mvwaddstr(h,15,1,"to place element on map.");
    mvwaddstr(h,18,1,"Press any key to continue");
    do
    {
        switch(ch=wgetch(h))
        {
            default:
            showhelp=false;
            curs_set(1);
            break;
        }
    }
    while(showhelp);
    delwin(h);
}

int yousure(void)
{
    WINDOW *q;
    bool showthis=true;
    int ch;
    q=newwin(4,35,7,4);
    box(q,0,0);
    mvwaddstr(q,1,1,"Press [Y/y] to save current map");
    mvwaddstr(q,2,1,"any key to cancel");
    wrefresh(q);
    do
    {
        switch(ch=wgetch(q))
        {
            case 121:
            case 89:
                showthis=false;
                delwin(q);
                return 1;
            break;
            default:
                showthis=false;
                delwin(q);
                return 0;
            break;
        }
    }
    while(showthis);
}