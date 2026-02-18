#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<time.h>
#include<curses.h>
#include<dirent.h>

#define MAP_MAX_Y 124
#define MAP_MAX_X 124

struct map_data
{
    int st_y;
    int st_x;

    int en0_y;
    int en0_x;

    int en1_y;
    int en1_x;

    int en2_y;
    int en2_x;

    int en3_y;
    int en3_x;

    char file_name[24];
    // directory name is hardcoded as "maps"
};


int main_menu(WINDOW *mmenu,WINDOW *map,void *p,int **d,bool gameloop);
int showhelp(WINDOW *helpwin);
int yousure(WINDOW *mmenu,WINDOW *q);
char *main_menu_map_select(WINDOW *loadmenu,WINDOW *map,int **d);
char *main_menu_new_map(WINDOW *map,int **d);
long wait_sec(long a);
long wait_nano(long a);

#include"maptools.h"

int main(void)
{
    initscr(); // init ncurses
    cbreak();
    scrollok(stdscr,false);
    noecho();
    curs_set(0);
    start_color();
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
    //                      
    const long WAIT_MENU=100000000;
    const long WAIT_60HZ=16666666;
    const int WELCOME_COLORSET=10;
    bool gameloop=false;
    bool mainmenu=false;
    bool gamepause=false;
    bool debug=true;
    bool welcome_screen=true;
    bool errinit=false;
    bool helpmenu=false;
    char errmsg[8];
    int **pMaparr;
    int menu_ret;
    int draw_ret;
    int ch;
    int welcome_i=0;
    int gety, getx; // terminal max y x
    int ed_y;
    int ed_x;
    int ed_symb;
    int ver0=0;
    int ver1=0;
    int ver2=1;
    struct map_data *pMapdata;
    struct dirent *map_folder_obj;
    WINDOW *map, *mmenu, *stats, *welcome, *helpwin;
    // after initialization
    getmaxyx(stdscr,gety,getx);
    stats=newwin(3,getx,0,0);
    welcome=newwin(gety-3,getx,3,0);
    box(stats,0,0);
    box(welcome,0,0);
    if(welcome_screen)
    {
        ch=0;
        nodelay(welcome,true);
            mvwaddstr(stats,1,1,"Set teminal window to at least 100x30!");
            wattron(welcome,COLOR_PAIR(4));
            mvwaddstr(welcome,2, 2,"        ####                      x  ");
            mvwaddstr(welcome,3, 2,"        #  ##                    x   ");
            mvwaddstr(welcome,4, 2,"        ####   #####  ##        x    ");
            mvwaddstr(welcome,5, 2,"        #  #    #  # #         x     ");
            mvwaddstr(welcome,6, 2,"        #  #    ###  #   #   dxb     ");
            mvwaddstr(welcome,7, 2,"        #  ###  #     ####   x       ");
            mvwprintw(welcome,7,3,"v%d.%d.%d",ver0,ver1,ver2);
            wattroff(welcome,COLOR_PAIR(4));
            if(gety<30 && getx<100)
            {
            mvwaddstr(welcome,11,2,"Please resize Your window to 100x30");
            mvwaddstr(welcome,12,2,"and restart RPG");
            }
        do
        {
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i));
            mvwaddstr(welcome,9,4,"Ed");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+1));
            mvwaddstr(welcome,9,6,"it");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+1));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+2));
            mvwaddstr(welcome,9,8,"or");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+2));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+3));
            mvwaddstr(welcome,9,10,"!!");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+3));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+4));
            mvwaddstr(welcome,9,12,"!!");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+4));
            welcome_i++;
            if(welcome_i>=6) welcome_i=1;
            switch(ch=wgetch(welcome))
            {
                case 410:
                    getmaxyx(stdscr,gety,getx);
                    wresize(welcome,gety-3,getx);
                    wresize(stats,3,getx);
                    wrefresh(welcome);
                    ch=0;
                break;
                case 10:
                case 32:
                case 48:
                    welcome_screen=false;
                break;
                default:
                touchwin(welcome);
                break;
            }
            wrefresh(stats);
            wrefresh(welcome);
            wait_nano(WAIT_MENU);
        }
        while(welcome_screen);
        nodelay(welcome,false);
    }
    wclear(stats);
    map=newwin(gety-3,getx,3,0);
    pMapdata=malloc(sizeof(struct map_data));
    if(pMapdata==NULL)
    {
        strcpy(errmsg,"RC0000");
        goto goto_error;
    }
    pMaparr=malloc(sizeof(int *)*MAP_MAX_Y*MAP_MAX_X);
    if(pMaparr==NULL)
    {
        strcpy(errmsg,"RC0001");
        goto goto_error;
    }
    for(int i=0;i<MAP_MAX_Y;i++)
    {
        pMaparr[i]=malloc(sizeof(int)*MAP_MAX_Y*MAP_MAX_X);
        if(pMaparr[i]==NULL)
        {
            strcpy(errmsg,"RC0002");
            goto goto_error;
        }
    }
    menu_ret=main_menu(mmenu,map,pMapdata,pMaparr,gameloop);
    if(menu_ret==-1)
    {
        goto goto_clean_exir;
    }
    // start editor loop after this point -----------------------------------------------
    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    curs_set(1);
    mvwprintw(stats,1,1,"%s",pMapdata->file_name);
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    box(stats,0,0);
    box(map,0,0);
    draw_ret=drawmap(map,pMapdata,pMaparr);
    gameloop=true;
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    box(map,0,0);
    ed_y=pMapdata->st_y;
    ed_x=pMapdata->st_x;
    wmove(map,ed_y,ed_x);
    //nodelay(map,true);  // initialize non ending loop
    do
    {
        switch((ch=wgetch(map)))
        {
            /*
            
            OK!!!
            STOP!!!!

            LOOP IS NOW ENABLED BY nodelay(map,true)

            play0.dir: 1 - left
                       2 - up
                       3 - down
                       4 - right

            */
            case 65: //UP
                wmove(map,ed_y--,ed_x);
                break;
            case 66: //DOWN
                wmove(map,ed_y++,ed_x);
                // down
                break;
            case 68: //LEFT
                wmove(map,ed_y,ed_x--);
                break;
            case 67: //RIGHT
                wmove(map,ed_y,ed_x++);
                break;
            case 32: // space key
                mvwaddch(map,ed_y,ed_x,' ');
                ed_x++;
                break;
            case 8: // backspace
                mvwaddch(map,ed_y,ed_x,' ');
                ed_x--;
                break;
            case 55:
                wclear(stats);
                box(stats,0,0);
                wrefresh(stats);
                redraw(map,stats,pMapdata,pMaparr);
                break;
            case 48:
                gameloop=false;
            case 49:
                gamepause=true;
                break;
            case 50:
                showhelp(helpwin);
                touchwin(map);
                break;

            
            //    122 - # - [Z] wooden box int 547 
            case 122:
                mvwaddch(map,ed_y,ed_x,547);
                ed_x++;
            break;

            //    120 - s - [X] stone wall int 1395
            case 120:
                mvwaddch(map,ed_y,ed_x,1395);
                ed_x++;
            break;

            //    99  - * - [C] mana orb int 2090
            case 99:
                mvwaddch(map,ed_y,ed_x,2090);
                ed_x++;
            break;

            //    118 - T - [V] tree int 2388
            case 118:
                mvwaddch(map,ed_y,ed_x,2388);
                ed_x++;
            break;

            //    113 - @ - [Q] set player start position int 1088
            case 113:
                mvwaddch(map,ed_y,ed_x,1088);
                curs_set(0);
                mvwaddstr(stats,1,1,"Player start changed!");
                box(stats,0,0);
                wrefresh(stats);
                wrefresh(map);
                wait_sec(2);
                wclear(stats);
                box(stats,0,0);
                curs_set(1);
            break;

            //    119 - % - [W] set 1st enemy position int 37
            case 119:
                mvwaddch(map,ed_y,ed_x,37);
                curs_set(0);
                mvwaddstr(stats,1,1,"Enemy 1 start changed!");
                box(stats,0,0);
                wrefresh(stats);
                wrefresh(map);
                wait_sec(2);
                wclear(stats);
                box(stats,0,0);
                curs_set(1);
            break;

            //    101 - K - [E] set 2nd enemy position int 75
            case 101:
                mvwaddch(map,ed_y,ed_x,75);
                curs_set(0);
                mvwaddstr(stats,1,1,"Enemy 2 start changed!");
                box(stats,0,0);
                wrefresh(stats);
                wrefresh(map);
                wait_sec(2);
                wclear(stats);
                box(stats,0,0);
                curs_set(1);
            break;

            //    104 - H - [R] set 3rd enemy position int 72
            case 104:
                mvwaddch(map,ed_y,ed_x,72);
                curs_set(0);
                mvwaddstr(stats,1,1,"Enemy 3 start changed!");
                box(stats,0,0);
                wrefresh(stats);
                wrefresh(map);
                wait_sec(2);
                wclear(stats);
                box(stats,0,0);
                curs_set(1);
            break;

            //    106 - O - [T] set 4th enemy position int 79
            case 106:
                mvwaddch(map,ed_y,ed_x,79);
                curs_set(0);
                mvwaddstr(stats,1,1,"Enemy 4 start changed!");
                box(stats,0,0);
                wrefresh(stats);
                wrefresh(map);
                wait_sec(2);
                wclear(stats);
                box(stats,0,0);
                curs_set(1);
            break;

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


            default:
                break;
        }
        // end of player interaction
        if(gamepause)
        {
            curs_set(0);
            menu_ret=main_menu(mmenu,map,pMapdata,pMaparr,gameloop);
            if(menu_ret==-1)
            {
                gameloop=false;
            }
            else if(menu_ret==2)
            {
                mvwprintw(map,5,5,"New map loading now");
                wclear(stats);
                box(stats,0,0);
                mvwprintw(stats,1,1,"%s",pMapdata->file_name);
                draw_ret=drawmap(map,pMapdata,pMaparr);
                touchwin(map);
                wrefresh(map);
                gamepause=false;
                curs_set(1);
            }
            else
            {
                touchwin(map);
                wrefresh(map);
                gamepause=false;
                curs_set(1);
            }
        }
        // check what is under cursor
        ed_symb=mvwinch(map,ed_y,ed_x);
        if(ed_symb==-1)
        {
            ed_y=pMapdata->st_y;
            ed_x=pMapdata->st_x;
        }
        // start world calculations after this point
        mvwprintw(stats,1,50,"        ");
        mvwprintw(stats,1,50,"%d",ed_symb);
        mvwprintw(stats,1,35,"         ");
        mvwprintw(stats,1,35,"y%3d x%3d",ed_y,ed_x);
        wrefresh(stats);
        wrefresh(map);
        //wait_nano(WAIT_60HZ);
    }
    while(gameloop);
    //end game after this point - error or clean exit
    goto_clean_exir: ;
    free(pMapdata);
    for(int i=0;i<MAP_MAX_Y;i++)
    {
        free(pMaparr[i]);
    }
    free(pMaparr);
    //error handler or clean exit - only use goto after deinitialization everything!!!!!
    goto_error: ;
    delwin(welcome);
    delwin(stats);
    endwin();
    if(errinit)
    {
        printf("\nerr= %s\n",errmsg);
    }
    return 0;
}

// return -1 ends game
// return 0 continues
// return 2 load new map
int main_menu(WINDOW *mmenu,WINDOW *map,void *p,int **d,bool gameloop)
{
    struct map_data *load0=p;
    char *menupos;
    int ret_exit;
    int ret_read;
    int ret_question;
    int ret_save;
    int ch;
    int cursory;
    int cursorx=2;
    bool menudajesz=true;
    WINDOW *loadmenu, *q_win;
    mmenu=newwin(10,18,5,5);
    box(mmenu,0,0);
    mvwaddstr(mmenu,0,2,"---=Menu=---");
    if(gameloop)
    {
        mvwaddstr(mmenu,3,3,"Continue");
        cursory=3;
    }
    else if(!gameloop)
    {
        mvwaddstr(mmenu,3,3,"New map");
        cursory=3;
    }
    mvwaddstr(mmenu,4,3,"Load map");
    mvwaddstr(mmenu,5,3,"Save map");
    mvwaddstr(mmenu,6,3,"Exit");
    mvwaddch(mmenu,cursory,cursorx,'>');
    wrefresh(mmenu);
    do
    {
        switch(ch=wgetch(mmenu))
        {
            case 65: // GO UP
                mvwaddch(mmenu,cursory,cursorx,' ');
                cursory--;
                if(cursory<=2)
                {
                    cursory++;
                }
                mvwaddch(mmenu,cursory,cursorx,'>');
                break;
            case 66: // GO DONW
                mvwaddch(mmenu,cursory,cursorx,' ');
                cursory++;
                if(cursory>=7)
                {
                    cursory--;
                }
                mvwaddch(mmenu,cursory,cursorx,'>');
                break;
            case 10: // ENTER
                if(cursory==6) // QUIT GAME
                {
                    menudajesz=false;
                    ret_exit=-1; // -1 will clean exit RPG
                }
                else if(cursory==3) // NEW MAP OR CONTINUE
                {
                    menudajesz=false;
                    ret_exit=0;
                }
                else if(cursory==4) // LOAD MAP
                {
                    menupos=main_menu_map_select(loadmenu,map,d);
                    if(menupos=="none")
                    {
                        touchwin(mmenu);
                        wrefresh(mmenu);
                        break;
                    }
                    if(gameloop=true)
                    {
                        ret_read=readmap(mmenu,p,d,menupos);
                        return 2;
                    }
                    wrefresh(map);
                    touchwin(mmenu);
                    wrefresh(mmenu);
                    ret_read=readmap(mmenu,p,d,menupos);
                    if(ret_read==-1)
                    {
                        menudajesz=false;
                        ret_exit=-1;
                    }
                    else
                    {
                        menudajesz=false;
                        ret_exit=1;
                    }
                }
                else if(cursory==5) // SAVE MAP
                {
                    if(gameloop)
                    {
                        ret_question=yousure(mmenu,q_win);
                        if(ret_question==0)
                        {
                            touchwin(mmenu);
                        }
                        else if(ret_question==1)
                        {
                            //readmap(mmenu,p,d,menupos);
                            ret_save=savemap(map,mmenu,p,d,menupos);
                            touchwin(mmenu);    
                            mvwaddstr(mmenu,5,3,"  Saved!");
                        }
                    }
                    else
                    {
                        mvwaddstr(mmenu,5,3,"Not Yet!");
                    }
                }
                break;
            default:
                break;
            
        }
        wrefresh(mmenu);
    }
    while(menudajesz);
    delwin(mmenu);
    return ret_exit;
}
int showhelp(WINDOW *h)
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
    bool showhelp=true;
    int ch;
    curs_set(0);
    h=newwin(20,35,4,4);
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

     mvwaddch(h,7,1,75);
    mvwaddstr(h,7,3,"- [E] set 2nd enemy position");

     mvwaddch(h,8,1,72);
    mvwaddstr(h,8,3,"- [R] set 3rd enemy position");

     mvwaddch(h,9,1,79);
    mvwaddstr(h,9,3,"- [T] set 4th enemy position");

    mvwaddstr(h,11,1,"Press key in bracket");
    mvwaddstr(h,12,1,"to place element on map.");
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
    return 0;
}

int yousure(WINDOW *mmenu,WINDOW *q)
{
    bool showthis=true;
    int ch;
    q=newwin(4,35,4,4);
    box(q,0,0);
    mvwaddstr(q,1,1,"Press [Y/y] to save current map");
    mvwaddstr(q,2,1,"any key to cancel");
    wrefresh(q);
    do
    {
        switch(ch=wgetch(mmenu))
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
    return 0;
}
char *main_menu_map_select(WINDOW *loadmenu,WINDOW *map,int **d)
{
    const char *map_folder="maps";
    struct dirent *map_folder_obj;
    char *menupos[256];
    char *ret_menu;
    int ch;
    int x=2;
    int y=4;
    int ret_exit;
    int map_folder_i=0;
    int map_count_i=2;
    bool loaddajesz=true;
    bool loaderr=false;
    bool giveptr=true;
    bool nomap=false;
    DIR *mapdir;
    loadmenu=newwin(10,30,6,6);
    box(loadmenu,0,0);
    mapdir=opendir(map_folder);
    if(mapdir==NULL)
    {   
        loaderr=true;
        goto goto_loaderr;
    }
    mvwaddstr(loadmenu,0,2,"---=Maps=---");
    while(map_folder_obj=readdir(mapdir)) // jeden plik w linux to 3 wpisy '.'[0] '..'[1] oraz 'plik'[2+n]
    {
        menupos[map_folder_i]=map_folder_obj->d_name;
        map_folder_i++;
    }
    for(int i=map_count_i;i<map_folder_i;i++)
    {
        mvwaddstr(loadmenu,i+2,3,menupos[i]);
        //mvwprintw(loadmenu,i+2,12,"%p",&menupos[i]);
    }
    closedir(mapdir);
    mvwaddstr(loadmenu,(map_folder_i+2),x+1,"Return");
    mvwaddch(loadmenu,y,x,'>');
    wrefresh(loadmenu);
    do
    {
        switch(ch=wgetch(loadmenu))
        {
            case 65:
                mvwaddch(loadmenu,y,x,' ');
                y--;
                if(y<=3)
                {
                    y++;
                }
                mvwaddch(loadmenu,y,x,'>');
                break;
            case 66:
                mvwaddch(loadmenu,y,x,' ');
                y++;
                if(y>=map_folder_i+3)
                {
                    y--;
                }
                mvwaddch(loadmenu,y,x,'>');
                break;
            case 10:
                if(y==(map_folder_i+2))
                {
                    loaddajesz=false;
                    ret_menu="none";
                    nomap=true;
                }
                else
                {
                    loaddajesz=false;
                    strcpy(ret_menu,menupos[y-2]);
                }
                break;
            default:
                break;
        }
        wrefresh(loadmenu);
    }
    while(loaddajesz);
    // exit ahead
    goto_loaderr: delwin(loadmenu);
    return ret_menu;
}

char *main_menu_new_map(WINDOW *map,int **d)
{
    const int maxname=8;
    bool write=true;
    int ch;
    int x=2;
    char *getname[12];
    WINDOW *q;
    q=newwin(4,35,4,4);
    box(q,0,0);
    mvwaddstr(q,1,1,"Provide new map name:");
    mvwaddstr(q,2,1,">________.map");
    wmove(q,2,x); 
    curs_set(2);
    do
    {
        switch(ch=wgetch(q))
        {
            case 10:
            write=false;
            break;
            default:
            mvwaddch(q,2,x,ch);
            wrefresh(q);
            x++;
            strcat(getname,ch);
            break;
        }
        if(x==maxname)
        {
            x=2;
            wmove(q,2,x);
        }
    }
    while(write);
    mvwaddstr(q,1,1,getname);
    wgetch(q);
    wrefresh(q);
    delwin(q);
    touchwin(map);
    return *getname;
}

long wait_nano(long nano_sec)
{
    long a;
    time_t start = clock();
    nanosleep((const struct timespec[]){{0, nano_sec}}, NULL);
    time_t end = clock();
    return a=end-start;
}

long wait_sec(long sec)
{
    long a;
    time_t start = clock();
    nanosleep((const struct timespec[]){{sec, 0}}, NULL);
    time_t end = clock();
    return a=end-start;
}