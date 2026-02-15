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
    bool is_hit;
};

long wait_sec(long a);
long wait_nano(long a);

#include"maptools.h"
#include"enemy.h"
#include"player.h"

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
    const char *map_folder="maps";
    const char *mapname="test";
    const char *mapname2="mordor";                
    const long WAIT_MENU=99999999;
    const long WAIT_60HZ=16666666;
    const int WELCOME_COLORSET=10;
    bool gameloop=false;
    bool gamepause=false;
    bool playermove=false;
    bool playeraction=false;
    bool fireball=false;
    bool debug=true;
    bool welcome_screen=true;
    bool errinit=false;
    bool isenemy0_dead=false;
    bool isenemy1_dead=false;
    bool playerdead=false;
    bool mapreload=false;
    char errmsg[8];
    char path[24];
    int **pMaparr;
    int menu_ret;
    int draw_ret;
    int enem0_ret;
    int enem1_ret;
    int ch;
    int whatmap=1;
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
    int ver1=0;
    int ver2=0;
    struct map_data *pMapdata;
    struct player play0={'@',100,0,1,1};
    struct enemy *enem0=malloc(sizeof(struct enemy));
    struct enemy *enem1=malloc(sizeof(struct enemy));
    struct dirent *map_folder_obj;
    WINDOW *map, *stats, *welcome;
    // after initialization
    getmaxyx(stdscr,gety,getx);
    stats=newwin(3,31,0,4);
    welcome=newwin(gety,getx,3,0);
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
            mvwprintw(welcome,7,3,"v%d.%d.%d",ver0,ver1,ver2);
            wattroff(welcome,COLOR_PAIR(4));
            if(gety<30 && getx<100)
            {
            mvwaddstr(welcome,11,2,"Please resize Your window to 100x30");
            mvwaddstr(welcome,12,2,"and restart RPG");
            }
            mvwaddstr(welcome,13,2,"Test version for debugging process");
            mvwaddstr(welcome,14,2,"Press any key to load :test.map:");
            mvwaddstr(welcome,16,2,"           in game:");
            mvwaddstr(welcome,17,2,"test.map         [1]");
            mvwaddstr(welcome,18,2,"mordor.map       [2]");
            mvwaddstr(welcome,17,2,"99 mana          [7]");
            mvwaddstr(welcome,19,2,"kill enemies     [9]");
            mvwaddstr(welcome,20,2,"destroy player   [8]");
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
                    wrefresh(welcome);
                    ch=0;
                break;
                case 10:
                case 32:
                case 48:
                    welcome_screen=false;
                break;
                default:
                break;
            }
            wrefresh(welcome);
            if(welcome_screen) wait_nano(WAIT_MENU);
        }
        while(welcome_screen);
    }
    delwin(welcome);
    map=newwin(gety,getx,3,0);
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
    // start game loop after this point -----------------------------------------------
    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    // load test map
    strcpy(path,map_folder);
    strcat(path,"/");
    strcat(path,mapname);
    strcpy(pMapdata->file_name,mapname);
    strcat(path,".map");
    draw_ret=readmap(pMapdata,pMaparr,mapname); //test.map will be hardcoded
    if(draw_ret==1)
    {
        goto goto_error;
    }
    touchwin(map);
    wrefresh(map);

    //FILL MAP WITH TREES BOUNDARIES
    redraw(map,pMapdata,pMaparr);
    mvwaddstr(map,2,2,"Loading:");
    wattron(map,COLOR_PAIR(11));
    mvwaddstr(map,2,11,pMapdata->file_name);
    wattroff(map,COLOR_PAIR(11));
    mvwaddstr(map,4,4,"Please wait");
    for(int i=15;i<19;i++)
        {
            mvwaddch(map,4,i,'.');
            wrefresh(map);
            wait_sec(1);
        }
    //
    drawmap(map,pMapdata,pMaparr);
    pl_y=pMapdata->st_y;
    pl_x=pMapdata->st_x;
    //enem0->symb='%';
    //enem1->symb='K';
    //enem0->ey=20;
    //enem0->ex=25;
    //enem1->ey=10;
    //enem1->ex=15;
    //enemy initialization
    enemyinit(map,enem0,0);
    enemyinit(map,enem1,1);
    //init game window after this point
    gameloop=true;
    box(stats,0,0);
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    wattron(map,COLOR_PAIR(4));
    mvwaddch(map,pl_y,pl_x,play0.symb);
    wattroff(map,COLOR_PAIR(4));
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
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0.symb);
                wattroff(map,COLOR_PAIR(4));
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
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0.symb);
                wattroff(map,COLOR_PAIR(4));
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
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0.symb);
                wattroff(map,COLOR_PAIR(4));
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
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0.symb);
                wattroff(map,COLOR_PAIR(4));
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
                play0.mana-=1;
                break;
                case 57:
                    enem0->is_dead=true;
                    enem1->is_dead=true;
                break;
                case 56:
                    play0.health=0;
                break;
                case 55:
                    play0.mana=99;
                break;
                case 49:
                    whatmap=1;
                    mapreload=true;
                break;
                case 50:
                    whatmap=2;
                    mapreload=true;
                break;
            case 48:
                gameloop=false;
                break;
            default:
                break;
        }
        // end of player interaction

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
                else if(f_ball_symb==2388)
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
                else if(f_ball_symb==2388)
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
                else if(f_ball_symb==2388)
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
                else if(f_ball_symb==2388)
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
            enem0_ret=enemymove(map,enem0);
            if(enem0_ret==1)
            {
                isenemy0_dead=true;
            }
            
        }
        if(!isenemy1_dead)
        {
            enem1_ret=enemymove(map,enem1);
            if(enem1_ret==1)
            {
                isenemy1_dead=true;
            }
        }

        if((enem0->is_hit)==true)
        {
            play0.health-=20;
            enem0->is_hit=false;
        }

        if((enem1->is_hit)==true)
        {
            play0.health-=20;
            enem1->is_hit=false;
        }

        if(play0.health<=0)
        {
            player_boom_anim(map,pl_y,pl_x);
            wait_sec(2);
            playerdead=true;
            gameloop=false;

        }
        // start world calculations after this point
        mvwaddstr(stats,1,1,pMapdata->file_name);
        wattron(stats,COLOR_PAIR(16));
        mvwprintw(stats,1,20,"*   ");
        mvwprintw(stats,1,20,"* %d",play0.mana);
        wattroff(stats,COLOR_PAIR(16));
        wattron(stats,COLOR_PAIR(13));
        mvwprintw(stats,1,25,"@    ");
        mvwprintw(stats,1,25,"@ %d",play0.health);
        wattroff(stats,COLOR_PAIR(13));
        wrefresh(stats);
        wrefresh(map);
        // reload procedure - only use to reload maps on new game
        if(mapreload)
        {
            wclear(stats);
            mvwaddch(stats,1,1,' ');
            wrefresh(stats);
            nodelay(map,false); // stop the loop before next tick
            if(whatmap==1)
            {
                draw_ret=readmap(pMapdata,pMaparr,mapname);
            }
            else if(whatmap==2)
            {
                draw_ret=readmap(pMapdata,pMaparr,mapname2);
            }
            if(draw_ret==1)
            {
                goto goto_error;
            }
            touchwin(map);
            wrefresh(map);

            //FILL MAP WITH TREES BOUNDARIES
            redraw(map,pMapdata,pMaparr);
            mvwaddstr(map,2,2,"Loading:");
            wattron(map,COLOR_PAIR(11));
            mvwaddstr(map,2,11,pMapdata->file_name);
            wattroff(map,COLOR_PAIR(11));
            mvwaddstr(map,4,4,"Please wait");
            for(int i=15;i<19;i++)
            {

                mvwaddch(map,4,i,'.');
                wrefresh(map);
                wait_sec(1);
            }
            //
            drawmap(map,pMapdata,pMaparr);
            pl_y=pMapdata->st_y;
            pl_x=pMapdata->st_x;
            //enem0->symb='%';
            //enem1->symb='K';
            //enem0->ey=20;
            //enem0->ex=25;
            //enem1->ey=10;
            //enem1->ex=15;
            //enemy initialization
            enemyinit(map,enem0,0);
            enemyinit(map,enem1,1);
            //init game window after this point
            gameloop=true;
            box(stats,0,0);
            wrefresh(stats);
            wrefresh(map);
            touchwin(map);
            wattron(map,COLOR_PAIR(4));
            mvwaddch(map,pl_y,pl_x,play0.symb);
            wattroff(map,COLOR_PAIR(4));
            mvwaddch(map,enem0->ey,enem0->ex,enem0->symb);
            mvwaddch(map,enem1->ey,enem1->ex,enem1->symb);
            srand(time(NULL));
            nodelay(map,true);  // initialize non ending loop
            mapreload=false;
        }
        fps=wait_nano(WAIT_60HZ);
    }
    while(gameloop);
    //error exit - after this point all malloc data deninitialze
    //DO NOT MALLOC something in the loop!!!!!
    goto_error: ;
    free(enem0);
    free(enem1);
    free(pMapdata);
    for(int i=0;i<MAP_MAX_Y;i++)
    {
        free(pMaparr[i]);
    }
    free(pMaparr);
    //only clean exit - only use goto after deinitialization everything!!!!!
    goto_clean_exit: ;
    delwin(welcome);
    delwin(stats);
    endwin();
    if(errinit)
    {
        printf("\nerr= %s\n",errmsg);
    }
    return 0;
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