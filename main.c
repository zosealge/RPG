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
    int maxy;
    int maxx;
    int st_y;
    int st_x;
    char file_name[24];
    // directory name is hardcoded as "maps"
};

struct player
{
    char symb;
    int health;
    int mana;
    int dir;
    int fireball_dir;
};

struct enemy
{
    char symb;
    int health;
    int dir;
    int ey;
    int ex;
    int e_symb;
    int fireball_dir;
    int fireball_y;
    int fireball_x;
    bool is_dead;
    bool is_shooting;
};


int main_menu(WINDOW *mmenu,WINDOW *map,void *p,int **d,bool gameloop);
char *main_menu_map_select(WINDOW *loadmenu,WINDOW *map,int **d);
long wait_sec(long a);
long wait_nano(long a);

#include"maptools.h"
#include"enemy.h"

int main(void)
{
    initscr(); // init ncurses
    cbreak();
    scrollok(stdscr,false);
    noecho();
    curs_set(0); // change when rendering map will be in place
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
    bool playermove=false;
    bool playeraction=false;
    bool fireball=false;
    bool debug=true;
    bool welcome_screen=true;
    bool errinit=false;
    bool isenemy0_dead=false;
    bool isenemy1_dead=false;
    char errmsg[8];
    int **pMaparr;
    int menu_ret;
    int draw_ret;
    int ch;
    int welcome_i=0;
    int gety, getx; // terminal max y x
    int fps;
    int pl_y;
    int pl_x;
    int pl_symb;
    int f_ball_y;
    int f_ball_x;
    int f_ball_symb;
    int ver0=0;
    int ver1=2;
    struct map_data *pMapdata;
    struct player play0={'@',100,0,4,4};
    struct enemy *enem0=malloc(sizeof(struct enemy));
    struct enemy *enem1=malloc(sizeof(struct enemy));
    //{'%',100,0,20,20,false};
    struct dirent *map_folder_obj;
    WINDOW *map, *mmenu, *stats, *welcome;
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
            wattron(welcome,COLOR_PAIR(4));
            mvwaddstr(welcome,2, 2,"        ####                      x  ");
            mvwaddstr(welcome,3, 2,"        #  ##                    x   ");
            mvwaddstr(welcome,4, 2,"        ####   #####  ##        x    ");
            mvwaddstr(welcome,5, 2,"        #  #    #  # #         x     ");
            mvwaddstr(welcome,6, 2,"        #  #    ###  #   #   dxb     ");
            mvwaddstr(welcome,7, 2,"        #  ###  #     ####   x       ");
            mvwprintw(welcome,7,3,"v%d.%d",ver0,ver1);
            wattroff(welcome,COLOR_PAIR(4));
            if(gety<30 && getx<100)
            {
            mvwaddstr(welcome,11,2,"Please resize Your window to 100x30");
            mvwaddstr(welcome,12,2,"and restart RPG");
            }
        do
        {
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i));
            mvwaddstr(welcome,9,4,"Ad");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+1));
            mvwaddstr(welcome,9,6,"ve");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+1));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+2));
            mvwaddstr(welcome,9,8,"nt");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+2));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+3));
            mvwaddstr(welcome,9,10,"ur");
            wattroff(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+3));
            wattron(welcome,COLOR_PAIR(WELCOME_COLORSET+welcome_i+4));
            mvwaddstr(welcome,9,12,"e!");
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
    }
    map=newwin(gety-3,getx,3,0);
    nodelay(welcome,false);
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
    // start game loop after this point -----------------------------------------------
    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    mvwprintw(stats,1,1,"%s",pMapdata->file_name);
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    box(stats,0,0);
    box(map,0,0);
    draw_ret=drawmap(map,pMapdata,pMaparr);
    pl_y=pMapdata->st_y;
    pl_x=pMapdata->st_x;
    //enemy initialization
    //if more enemies need the init
    //involve for() loop
    enem0->symb='%';
    enem0->health=100;
    enem0->dir=1;
    enem0->ey=20;
    enem0->ex=20;
    enem0->is_dead=false;
    enem0->is_shooting=false;
    //
    enem1->symb='K';
    enem1->health=100;
    enem1->dir=1;
    enem1->ey=25;
    enem1->ex=27;
    enem1->is_dead=false;
    enem1->is_shooting=false;
    //init game window after this point
    gameloop=true;
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    box(map,0,0);
    mvwaddch(map,pl_y,pl_x,play0.symb);
    mvwaddch(map,enem0->ey,enem0->ex,enem0->symb);
    mvwaddch(map,enem1->ey,enem1->ex,enem1->symb);
    srand(time(NULL));
    nodelay(map,true);  // initialize non ending loop
    do
    {
        switch((ch=wgetch(map)))
        {
            /*
            
            OK!!!
            STOP!!!!

            LOOP IS NOW ENABLED BY nodelay(map,true)

            Controls represented as int
            UP    - 65
            DOWN  - 66
            LEFT  - 68
            RIGHT - 67

            int value map elements

            4194417 - ramka up/down
            4194424 - ramka left/right
            547 - (2)colored box
            2090 - (4)colored mana

            play0.dir: 1 - left
                       2 - up
                       3 - down
                       4 - right

            */
            case 65: //UP
                play0.dir=2;
                pl_symb=mvwinch(map,pl_y-1,pl_x);
                if(pl_symb==4194417) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2090)
                {
                    play0.mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_y--;
                mvwaddch(map,pl_y,pl_x,play0.symb);
                break;
            case 66: //DOWN
                play0.dir=3;
                pl_symb=mvwinch(map,pl_y+1,pl_x);
                if(pl_symb==4194417) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2090)
                {
                    play0.mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_y++;
                mvwaddch(map,pl_y,pl_x,play0.symb);
                break;
            case 68: //LEFT
                play0.dir=1;
                pl_symb=mvwinch(map,pl_y,pl_x-1);
                if(pl_symb==4194424) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2090)
                {
                    play0.mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_x--;
                mvwaddch(map,pl_y,pl_x,play0.symb);
                break;
            case 67: //RIGHT
                play0.dir=4;
                pl_symb=mvwinch(map,pl_y,pl_x+1);
                if(pl_symb==4194424) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2090)
                {
                    play0.mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_x++;
                mvwaddch(map,pl_y,pl_x,play0.symb);
                break;
            case 32: // space key
                /*
                       1 - left
                       2 - up
                       3 - down
                       4 - right
                */
                if(play0.mana==0) break;
                if(fireball) break;
                play0.fireball_dir=play0.dir;
                f_ball_y=pl_y;
                f_ball_x=pl_x;
                if(play0.fireball_dir==1)
                {
                    f_ball_x--;
                }
                else if(play0.fireball_dir==2)
                {
                    f_ball_y--;
                }
                else if(play0.fireball_dir==3)
                {
                    f_ball_y++;
                }
                else if(play0.fireball_dir==4)
                {
                    f_ball_x++;
                }
                fireball=true;
                break;
                case 57:
                    enem0->is_dead=true;
                    enem1->is_dead=true;
                break;
            case 48:
                gameloop=false;
            case 49:
                gamepause=true;
                break;
            default:
                break;
        }
        // end of player interaction
        if(gamepause)
        {
            menu_ret=main_menu(mmenu,map,pMapdata,pMaparr,gameloop);
            if(menu_ret==-1) gameloop=false;
            else if(menu_ret==2)
            {
                mvwprintw(map,5,5,"New map loading now");
                wclear(stats);
                box(stats,0,0);
                mvwprintw(stats,1,1,"%s",pMapdata->file_name);
                nodelay(map,false);
                draw_ret=drawmap(map,pMapdata,pMaparr);
                nodelay(map,true);
                // redraw map - start from beggining
                touchwin(map);
                wrefresh(map);
                gamepause=false;
            } 
            else
            {
                touchwin(map);
                wrefresh(map);
                gamepause=false;
            }
        }
        /*
        TODO / known bugs

        - int ch show -1 value most of the time - better if will show last pressed keys
        - fireball floats quicker up and down, slower in sides
        - fireball won't kill enemy
        - if player health reaches 0 game pauses instead GAME OVER screen

        */
        // start of world interaction after this point
        if(fireball)
        {
            //1 - left
            //2 - up
            //3 - down
            //4 - right
            if(play0.fireball_dir==1)
            {
                mvwaddch(map,f_ball_y,f_ball_x,' ');
                f_ball_x--;
                mvwaddch(map,f_ball_y,f_ball_x,'-');
                f_ball_symb=mvwinch(map,f_ball_y,f_ball_x-1);
                if(f_ball_symb==4194424)
                {
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==37)
                {
                    enem0->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==75)
                {
                    enem1->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
            }
            else if(play0.fireball_dir==2)
            {
                mvwaddch(map,f_ball_y,f_ball_x,' ');
                f_ball_y--;
                mvwaddch(map,f_ball_y,f_ball_x,'|');
                f_ball_symb=mvwinch(map,f_ball_y-1,f_ball_x);
                if(f_ball_symb==4194417)
                {
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==37)
                {
                    enem0->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==75)
                {
                    enem1->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
            }
            else if(play0.fireball_dir==3)
            {
                mvwaddch(map,f_ball_y,f_ball_x,' ');
                f_ball_y++;
                mvwaddch(map,f_ball_y,f_ball_x,'|');
                f_ball_symb=mvwinch(map,f_ball_y+1,f_ball_x);
                if(f_ball_symb==4194417)
                {
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==37)
                {
                    enem0->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==75)
                {
                    enem1->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
            }
            else if(play0.fireball_dir==4)
            {
                mvwaddch(map,f_ball_y,f_ball_x,' ');
                f_ball_x++;
                mvwaddch(map,f_ball_y,f_ball_x,'-');
                f_ball_symb=mvwinch(map,f_ball_y,f_ball_x+1);
                if(f_ball_symb==4194424)
                {
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==37)
                {
                    enem0->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
                else if(f_ball_symb==75)
                {
                    enem1->is_dead=true;
                    fireball=false;
                    mvwaddch(map,f_ball_y,f_ball_x,' ');
                }
            }
        }
        // enemy calculations
        if(!isenemy0_dead)
        {
            if(enemymove(map,enem0)==1)
            {
                isenemy0_dead=true;
            }
            
        }
        if(!isenemy1_dead)
        {
            if(enemymove(map,enem1)==1)
            {
                isenemy1_dead=true;
            }
        }

        if(play0.health<=0)
        {
            gamepause=true;
        }
        // start world calculations after this point
        wattron(stats,COLOR_PAIR(16));
        mvwprintw(stats,1,20,"*   ");
        mvwprintw(stats,1,20,"* %d",play0.mana);
        wattroff(stats,COLOR_PAIR(16));
        wattron(stats,COLOR_PAIR(13));
        mvwprintw(stats,1,25,"@    ");
        mvwprintw(stats,1,25,"@ %d",play0.health);
        wattroff(stats,COLOR_PAIR(13));
        mvwprintw(stats,1,50,"        ");
        mvwprintw(stats,1,50,"%d",pl_symb);
        wrefresh(stats);
        wrefresh(map);
        fps=wait_nano(WAIT_60HZ);
    }
    while(gameloop);
    //end game after this point - error or clean exit
    goto_clean_exir: ;
    free(enem0);
    free(enem1);
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
    int ch;
    int cursory;
    int cursorx=2;
    bool menudajesz=true;
    WINDOW *loadmenu;
    mmenu=newwin(10,18,5,5);
    box(mmenu,0,0);
    nodelay(mmenu,false);
    mvwaddstr(mmenu,0,2,"---=Menu=---");
    if(gameloop)
    {
    mvwaddstr(mmenu,3,3,"Continue");
    cursory=3;
    }
    else cursory=4;
    mvwaddstr(mmenu,4,3,"New Game");
    mvwaddstr(mmenu,5,3,"Exit");
    mvwaddch(mmenu,cursory,cursorx,'>');
    wrefresh(mmenu);
    do
    {
        switch(ch=wgetch(mmenu))
        {
            case 65:
                mvwaddch(mmenu,cursory,cursorx,' ');
                cursory--;
                if(gameloop)
                {
                    if(cursory<=2)
                    {
                        cursory++;
                    }
                }
                else
                {
                    if(cursory<=3)
                    {
                        cursory++;
                    }
                }
                mvwaddch(mmenu,cursory,cursorx,'>');
                break;
            case 66:
                mvwaddch(mmenu,cursory,cursorx,' ');
                cursory++;
                if(cursory>=6)
                {
                    cursory--;
                }
                mvwaddch(mmenu,cursory,cursorx,'>');
                break;
            case 10:
                if(cursory==5)
                {
                    menudajesz=false;
                    ret_exit=-1; // -1 will clean exit RPG
                }
                else if(cursory==3)
                {
                    menudajesz=false;
                    ret_exit=0;
                }
                else if(cursory==4)
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
                    //mvwprintw(mmenu,6,2,"sel: %p",ret_mapselect); // d_name position is 2 for "real" files
                    //menupos=rozmowa();
                    //mvwprintw(mmenu,7,2,"%s",menupos);
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
                break;
            default:
                break;
            
        }
        wrefresh(mmenu);
    }
    while(menudajesz);
    nodelay(mmenu,true);
    delwin(mmenu);
    return ret_exit;
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
    nodelay(loadmenu,false);
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