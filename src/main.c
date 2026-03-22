#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<string.h>
#include<stdint.h>
#include<time.h>
#include<curses.h>
#include<dirent.h>

#define MAP_MAX_SQ 124
#define NSLEEP(n) nanosleep((const struct timespec[]){{0, (n)}}, NULL)
#define SSLEEP(n) nanosleep((const struct timespec[]){{(n), 0}}, NULL)

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
    int f_ball_y;
    int f_ball_x;
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

enum global_directions
{
    D_LEFT=1,
    D_UP=2,
    D_DOWN=3,
    D_RIGHT=4,
};

enum key_press
{
    KEY_BUTTON_UP=259,
    KEY_BUTTON_DOWN=258,
    KEY_BUTTON_RIGHT=261,
    KEY_BUTTON_LEFT=260,
    KEY_BUTTON_SPACE=32,
    KEY_BUTTON_TILDE=96,
    KEY_BUTTON_SEVEN=55,
    KEY_BUTTON_EIGHT=56,
    KEY_BUTTON_NINE=57,
    KEY_BUTTON_ZERO=48
};


#include"maptools.h"
#include"enemy.h"
#include"player.h"
#include"menu.h"
#include"init.h"

int main(void)
{
    initscr(); // init ncurses
    cbreak();
    scrollok(stdscr,false);
    noecho();
    noqiflush();
    curs_set(0); // change when rendering map will be in place
    start_color();
    initrpgcolor();             
    const long WAIT_60HZ=16666666L;
    bool gameloop=false;
    bool menupause=false;
    bool fireball=false;
    bool errinit=false;
    bool isenemy0_dead=false;
    bool isenemy1_dead=false;
    bool isenemy2_dead=false;
    bool isenemy3_dead=false;
    bool mapreload=false;
    char errmsg[8];
    //int **pMaparr;
    int fireball_ret;
    int menu_ret;
    int newmenu_ret;
    int draw_ret;
    int enem0_ret;
    int enem1_ret;
    int enem2_ret;
    int enem3_ret;
    int ch;
    int gety, getx; // terminal max y x
    int pl_y;
    int pl_x;
    int pl_symb;

    uint8_t *pMaparr;

    //int f_ball_symb;
    struct map_data *pMapdata;
    struct player *play0=malloc(sizeof(struct player));
    if(play0==NULL)
    {
        strncpy(errmsg,"RC0001",sizeof(errmsg));
        goto goto_error;
    }
    struct enemy *enem0=malloc(sizeof(struct enemy));
    if(enem0==NULL)
    {
        strncpy(errmsg,"RC00A2",sizeof(errmsg));
        goto goto_error;
    }
    struct enemy *enem1=malloc(sizeof(struct enemy));
    if(enem1==NULL)
    {
        strncpy(errmsg,"RC00B2",sizeof(errmsg));
        goto goto_error;
    }
    struct enemy *enem2=malloc(sizeof(struct enemy));
    if(enem2==NULL)
    {
        strncpy(errmsg,"RC00A2",sizeof(errmsg));
        goto goto_error;
    }
    struct enemy *enem3=malloc(sizeof(struct enemy));
    if(enem3==NULL)
    {
        strncpy(errmsg,"RC00B2",sizeof(errmsg));
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
        strncpy(errmsg,"RC0010",sizeof(errmsg));
        goto goto_error;
    }
    //uint16_t *mem_bg=malloc((MAP_MAX_TILES_SQ*MAP_MAX_TILES_SQ)*sizeof(uint16_t));

    pMaparr=malloc((MAP_MAX_SQ*MAP_MAX_SQ)*sizeof(uint8_t));
    //defines a pMaparr[y*MAP_MAX_SQ+x] array
    //can be used without type casting
    //like map[y*MAP_MAX_SQ+x]=5;
    //dynamic array need to be equal in y as in x
    if(pMaparr==NULL)
    {
        strncpy(errmsg,"RC0011",sizeof(errmsg));
        goto goto_error;
    }
    /*
    pMaparr=malloc(sizeof(int *)*MAP_MAX_Y*MAP_MAX_X);
    if(pMaparr==NULL)
    {
        strcpy(errmsg,"RC0011");
        goto goto_error;
    }
    for(int i=0;i<MAP_MAX_Y;i++)
    {
        pMaparr[i]=malloc(sizeof(int)*MAP_MAX_Y*MAP_MAX_X);
        if(pMaparr[i]==NULL)
        {
            strcpy(errmsg,"RC0012");
            goto goto_error;
        }
    }
    */

    newmenu_ret=newmenu(pMapdata);
    if(newmenu_ret==1)
    {
        goto goto_clean_exit;
    }
    // start game loop after this point -----------------------------------------------
    // --------------------------------------------------------------------------------
    // --------------------------------------------------------------------------------
    draw_ret=readmap(pMapdata,pMaparr);
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
            SSLEEP(1);
        }
    drawmap(map,pMaparr,pMapdata);
    pl_y=pMapdata->st_y;
    pl_x=pMapdata->st_x;
    play0_init(play0);
    enemyinit(enem0,pMapdata,0);
    enemyinit(enem1,pMapdata,1);
    enemyinit(enem2,pMapdata,2);
    enemyinit(enem3,pMapdata,3);
    //init game window after this point
    gameloop=true;
    box(stats,0,0);
    wrefresh(stats);
    wrefresh(map);
    touchwin(map);
    wattron(map,COLOR_PAIR(4));
    mvwaddch(map,pl_y,pl_x,play0->symb);
    wattroff(map,COLOR_PAIR(4));
    mvwaddch(map,enem0->ey,enem0->ex,enem0->symb);
    mvwaddch(map,enem1->ey,enem1->ex,enem1->symb);
    mvwaddch(map,enem2->ey,enem2->ex,enem2->symb);
    mvwaddch(map,enem3->ey,enem3->ex,enem3->symb);
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

            */
            case KEY_BUTTON_UP:
                play0->dir=D_UP;
                pl_symb=mvwinch(map,pl_y-1,pl_x);
                if(pl_symb==4194417) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2388) break;
                else if(pl_symb==1395) break;
                else if(pl_symb==2090)
                {
                    play0->mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_y--;
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0->symb);
                wattroff(map,COLOR_PAIR(4));
                break;
            case KEY_BUTTON_DOWN:
                play0->dir=D_DOWN;
                pl_symb=mvwinch(map,pl_y+1,pl_x);
                if(pl_symb==4194417) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2388) break;
                else if(pl_symb==1395) break;
                else if(pl_symb==2090)
                {
                    play0->mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_y++;
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0->symb);
                wattroff(map,COLOR_PAIR(4));
                break;
            case KEY_BUTTON_LEFT:
                play0->dir=D_LEFT;
                pl_symb=mvwinch(map,pl_y,pl_x-1);
                if(pl_symb==4194424) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2388) break;
                else if(pl_symb==1395) break;
                else if(pl_symb==2090)
                {
                    play0->mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_x--;
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0->symb);
                wattroff(map,COLOR_PAIR(4));
                break;
            case KEY_BUTTON_RIGHT:
                play0->dir=D_RIGHT;
                pl_symb=mvwinch(map,pl_y,pl_x+1);
                if(pl_symb==4194424) break;
                else if(pl_symb==547) break;
                else if(pl_symb==2388) break;
                else if(pl_symb==1395) break;
                else if(pl_symb==2090)
                {
                    play0->mana++;
                }
                mvwprintw(map,pl_y,pl_x," ");
                pl_x++;
                wattron(map,COLOR_PAIR(4));
                mvwaddch(map,pl_y,pl_x,play0->symb);
                wattroff(map,COLOR_PAIR(4));
                break;
            case KEY_BUTTON_SPACE: 
                if(play0->mana==0) break;
                if(fireball) break;
                play0->fireball_dir=play0->dir;
                play0->f_ball_y=pl_y;
                play0->f_ball_x=pl_x;
                if(play0->fireball_dir==D_LEFT)
                {
                    play0->f_ball_x--;
                }
                else if(play0->fireball_dir==D_UP)
                {
                    play0->f_ball_y--;
                }
                else if(play0->fireball_dir==D_DOWN)
                {
                    play0->f_ball_y++;
                }
                else if(play0->fireball_dir==D_RIGHT)
                {
                    play0->f_ball_x++;
                }
                fireball=true;
                play0->mana-=1;
                break;
                case KEY_BUTTON_NINE:
                    enem0->is_dead=true;
                    enem1->is_dead=true;
                    enem2->is_dead=true;
                    enem3->is_dead=true;
                break;
                case KEY_BUTTON_EIGHT:
                    play0->health=0;
                break;
                case KEY_BUTTON_SEVEN:
                    play0->mana=99;
                break;

                case KEY_BUTTON_ZERO:
                    gameloop=false;
                break;
                case KEY_BUTTON_TILDE:
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
            fireball_ret=fireballaction(map,play0);
                 if(fireball_ret==1)
            {
                fireball=false;
            }
            else if(fireball_ret==10)
            {
                fireball=false;
                enem0->is_dead=true;
            }
            else if(fireball_ret==11)
            {
                fireball=false;
                enem1->is_dead=true;
            }
            else if(fireball_ret==12)
            {
                fireball=false;
                enem2->is_dead=true;
            }
            else if(fireball_ret==13)
            {
                fireball=false;
                enem3->is_dead=true;
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
        if(!isenemy2_dead)
        {
            enem2_ret=enemymove(map,enem2);
            if(enem2_ret==1)
            {
                isenemy2_dead=true;
            }
            
        }
        if(!isenemy3_dead)
        {
            enem3_ret=enemymove(map,enem3);
            if(enem3_ret==1)
            {
                isenemy3_dead=true;
            }
        }

        if((enem0->is_hit)==true)
        {
            play0->health-=20;
            enem0->is_hit=false;
        }

        if((enem1->is_hit)==true)
        {
            play0->health-=20;
            enem1->is_hit=false;
        }
        if((enem2->is_hit)==true)
        {
            play0->health-=20;
            enem2->is_hit=false;
        }

        if((enem3->is_hit)==true)
        {
            play0->health-=20;
            enem3->is_hit=false;
        }

        if(play0->health<=0)
        {
            player_boom_anim(map,pl_y,pl_x);
            SSLEEP(2);
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
        mvwprintw(stats,1,20,"* %d",play0->mana);
        wattroff(stats,COLOR_PAIR(16));
        wattron(stats,COLOR_PAIR(13));
        mvwprintw(stats,1,25,"@    ");
        mvwprintw(stats,1,25,"@ %d",play0->health);
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
            nodelay(map,false); // stop the loop before next tick
            draw_ret=readmap(pMapdata,pMaparr);
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
                SSLEEP(1);
            }
            //
            drawmap(map,pMaparr,pMapdata);
            play0->health=100;
            play0->mana=0;
            pl_y=pMapdata->st_y;
            pl_x=pMapdata->st_x;
            //enemy initialization
            enemyinit(enem0,pMapdata,0);
            enemyinit(enem1,pMapdata,1);
            enemyinit(enem2,pMapdata,2);
            enemyinit(enem3,pMapdata,3);
            isenemy0_dead=false;
            isenemy1_dead=false;
            isenemy2_dead=false;
            isenemy3_dead=false;
            //init game window after this point
            gameloop=true;
            box(stats,0,0);
            wrefresh(stats);
            wrefresh(map);
            touchwin(map);
            wattron(map,COLOR_PAIR(4));
            mvwaddch(map,pl_y,pl_x,play0->symb);
            wattroff(map,COLOR_PAIR(4));
            mvwaddch(map,enem0->ey,enem0->ex,enem0->symb);
            mvwaddch(map,enem1->ey,enem1->ex,enem1->symb);
            mvwaddch(map,enem2->ey,enem2->ex,enem2->symb);
            mvwaddch(map,enem3->ey,enem3->ex,enem3->symb);
            nodelay(map,true);  // initialize non ending loop
            mapreload=false;
        }
        NSLEEP(WAIT_60HZ);
    }
    while(gameloop);
    //error exit - after this point all malloc data deninitialze
    goto_error: ;
    free(play0);
    free(enem0);
    free(enem1);
    free(enem2);
    free(enem3);
    free(pMapdata);
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