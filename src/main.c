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
#include"menu.h"

int main(void)
{
    initscr(); // init ncurses
    cbreak();
    scrollok(stdscr,false);
    noecho();
    noqiflush();
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
    const long WAIT_60HZ=16666666;
    bool gameloop=false;
    bool menupause=false;
    bool fireball=false;
    bool errinit=false;
    bool isenemy0_dead=false;
    bool isenemy1_dead=false;
    bool mapreload=false;
    char errmsg[8];
    int **pMaparr;
    int menu_ret;
    int newmenu_ret;
    int draw_ret;
    int enem0_ret;
    int enem1_ret;
    int ch;
    int gety, getx; // terminal max y x
    int pl_y;
    int pl_x;
    int pl_symb;
    int f_ball_y;
    int f_ball_x;
    int f_ball_symb;
    struct map_data *pMapdata;
    struct player play0={'@',100,0,1,1};
    struct enemy *enem0=malloc(sizeof(struct enemy));
    if(enem0==NULL)
    {
        strcpy(errmsg,"RC0001");
        goto goto_error;
    }
    struct enemy *enem1=malloc(sizeof(struct enemy));
    if(enem1==NULL)
    {
        strcpy(errmsg,"RC0001");
        goto goto_error;
    }
    WINDOW *map, *stats;
    // after initialization
    getmaxyx(stdscr,gety,getx);
    stats=newwin(3,31,0,4);
    welcomescreen(); // welcome screen init 
    map=newwin(gety,getx,3,0);
    pMapdata=malloc(sizeof(struct map_data));
    if(pMapdata==NULL)
    {
        strcpy(errmsg,"RC0001");
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
            strcpy(errmsg,"RC0001");
            goto goto_error;
        }
    }
    newmenu_ret=newmenu(pMapdata);
    if(newmenu_ret==1)
    {
        goto goto_clean_exit;
    }
    // start game loop after this point -----------------------------------------------
    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    draw_ret=readmap(pMapdata,pMaparr,(pMapdata->file_name));
    if(draw_ret==1)
    {
        goto goto_error;
    }
    touchwin(map);
    wrefresh(map);

    //FILL MAP WITH TREES BOUNDARIES
    redraw(map);
    mvwaddstr(map,2,3,"Loading:");
    wattron(map,COLOR_PAIR(11));
    mvwaddstr(map,2,12,pMapdata->file_name);
    wattroff(map,COLOR_PAIR(11));
    mvwaddstr(map,4,4,"Please wait");
    for(int i=15;i<19;i++)
        {
            mvwaddch(map,4,i,'.');
            wrefresh(map);
            wait_sec(1);
        }
    drawmap(map,pMaparr);
    pl_y=pMapdata->st_y;
    pl_x=pMapdata->st_x;
    enemyinit(enem0,0);
    enemyinit(enem1,1);
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
    keypad(map,true);
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
            case 259: //UP
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
            case 258: //DOWN
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
            case 260: //LEFT
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
            case 261: //RIGHT
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

                case 48:
                    gameloop=false;
                break;
                case 96:
                    menupause=true;
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
            gameloop=false;
            menu_ret=mainmenu(gameloop,pMapdata);
            if(menu_ret==2)
            {
                mapreload=false;
                menupause=false;
            }
            else
            {
                mapreload=true;
                gameloop=true;
            }

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

        if(menupause)
        {
            nodelay(map,false);
            menu_ret=mainmenu(gameloop,pMapdata);
            if(menu_ret==2)
            {
                gameloop=false;
            }
            else if(menu_ret==1)
            {
                mapreload=true;
            }
            touchwin(map);
            menupause=false;
            nodelay(map,true);
        }
        // reload procedure - only use to reload maps on new game
        if(mapreload)
        {
            //mvwaddch(stats,1,1,' ');
            //wrefresh(stats);
            nodelay(map,false); // stop the loop before next tick
            draw_ret=readmap(pMapdata,pMaparr,(pMapdata->file_name));
            if(draw_ret==1)
            {
                goto goto_error;
            }
            touchwin(map);
            wrefresh(map);
            undraw(map,stats);

            //FILL MAP WITH TREES BOUNDARIES
            redraw(map);
            mvwaddstr(map,2,3,"Loading:");
            wattron(map,COLOR_PAIR(11));
            mvwaddstr(map,2,12,pMapdata->file_name);
            wattroff(map,COLOR_PAIR(11));
            mvwaddstr(map,4,4,"Please wait");
            for(int i=15;i<19;i++)
            {

                mvwaddch(map,4,i,'.');
                wrefresh(map);
                wait_sec(1);
            }
            //
            drawmap(map,pMaparr);
            play0.health=100;
            play0.mana=0;
            pl_y=pMapdata->st_y;
            pl_x=pMapdata->st_x;
            //enemy initialization
            enemyinit(enem0,0);
            enemyinit(enem1,1);
            isenemy0_dead=false;
            isenemy1_dead=false;
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
            nodelay(map,true);  // initialize non ending loop
            mapreload=false;
        }
        wait_nano(WAIT_60HZ);
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