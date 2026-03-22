#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<stdint.h>
#include<string.h>
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
    KEY_BUTTON_BACKSPACE=263,
    KEY_BUTTON_TILDE=96,
    KEY_BUTTON_SEVEN=55,
    KEY_BUTTON_EIGHT=56,
    KEY_BUTTON_NINE=57,
    KEY_BUTTON_ZERO=48,
    KEY_BUTTON_ONE=49,
    KEY_BUTTON_Z=122,
    KEY_BUTTON_X=120,
    KEY_BUTTON_C=99,
    KEY_BUTTON_V=118,
    KEY_BUTTON_Q=113,
    KEY_BUTTON_W=119,
    KEY_BUTTON_E=101,
    KEY_BUTTON_R=114,
    KEY_BUTTON_T=116
};

#include"maptools.h"
#include"menu.h"
#include"init.h"

int main(void)
{
    initscr(); // init ncurses
    cbreak();
    scrollok(stdscr,false);
    noecho();
    noqiflush();
    curs_set(1); // editor mode 1
    start_color();
    initrpgcolor();
    //                  
    //const long WAIT_60HZ=16666666;
    bool gameloop=false;
    bool menupause=false;
    bool mapreload=false;
    bool mapsave=false;
    bool errinit=false;
    bool help=false;
    char errmsg[8];
    uint8_t *pMaparr;
    int menu_ret;
    int newmenu_ret;
    int draw_ret;
    int save_ret;
    int ch;
    int gety, getx; // terminal max y x
    int pl_y;
    int pl_x;
    int pl_symb;
    struct map_data *pMapdata;
    WINDOW *map, *stats, *name;
    // after initialization
    getmaxyx(stdscr,gety,getx);
    stats=newwin(3,31,0,4);
    name=newwin(3,14,0,36);
    map=newwin(gety,getx,3,0);
    editorscreen(); // editor screen init
    pMapdata=malloc(sizeof(struct map_data));
    if(pMapdata==NULL)
    {
        strncpy(errmsg,"RCM001",sizeof(errmsg));
        goto goto_error;
    }
    newmenu_ret=newmenu(pMapdata);
    if(newmenu_ret==1)
    {
        goto goto_clean_exit;
    }
    pMaparr=malloc((MAP_MAX_SQ*MAP_MAX_SQ)*sizeof(uint8_t));
    if(pMaparr==NULL)
    {
        strncpy(errmsg,"RCM002",sizeof(errmsg));
        goto goto_error;
    }
    // start editor loop after this point----------------------------------------------
    // editor uses nodelay(map,false)--------------------------------------------------
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
    //init editor window after this point - use "gameloop" name for compatibility
    pl_y=pMapdata->st_y;
    pl_x=pMapdata->st_x;
    gameloop=true;
    box(stats,0,0);
    box(name,0,0);
    mvwaddstr(name,1,1,pMapdata->file_name);
    wrefresh(name);
    pl_symb=mvwinch(map,pl_y,pl_x);
    mvwprintw(stats,1,1,"    y     x          symbol");
    mvwprintw(stats,1,1,"%3d y %3d x %8d symbol",pl_y,pl_x,pl_symb);
    wrefresh(stats);
    mvwaddch(map,pl_y,pl_x,1088);
    wmove(map,pl_y,pl_x);
    wrefresh(map);
    touchwin(map);
    nodelay(map,false);  // no auto-loop, wait for key press
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
            case KEY_BUTTON_UP: //UP
                pl_y--;
                //
                break;
            case KEY_BUTTON_DOWN: //DOWN
                pl_y++;
                //
                break;
            case KEY_BUTTON_LEFT: //LEFT
                pl_x--;
                //
                break;
            case KEY_BUTTON_RIGHT: //RIGHT
                pl_x++;
                //
                break;
            case KEY_BUTTON_Z: // z - # 547
                    mvwaddch(map,pl_y,pl_x,547);
                    pl_x++;
                break;
            case KEY_BUTTON_X: // x - s 1395
                    mvwaddch(map,pl_y,pl_x,1395);
                    pl_x++;
                break;
            case KEY_BUTTON_C: // c - * 2090
                    mvwaddch(map,pl_y,pl_x,2090);
                    pl_x++;
                break;
            case KEY_BUTTON_V: // v - T 2388
                    mvwaddch(map,pl_y,pl_x,2388);
                    pl_x++;
                break;
            case KEY_BUTTON_Q: // q - @ 1088
                    mvwaddch(map,(pMapdata->st_y),(pMapdata->st_x),' ');
                    mvwaddch(map,pl_y,pl_x,1088);
                    pMapdata->st_y=pl_y;
                    pMapdata->st_x=pl_x;
                    mvwprintw(stats,1,13,"Player Set      ");
                    wrefresh(stats);
                    wmove(map,pl_y,pl_x);
                    wrefresh(map);
                    SSLEEP(1);
                break;
            case KEY_BUTTON_W: // w - enemy0 % 37
                    mvwaddch(map,(pMapdata->en0_y),(pMapdata->en0_x),' ');
                    mvwaddch(map,pl_y,pl_x,37);
                    pMapdata->en0_y=pl_y;
                    pMapdata->en0_x=pl_x;
                    mvwprintw(stats,1,13,"Enemy 1 Set     ");
                    wrefresh(stats);
                    wmove(map,pl_y,pl_x);
                    wrefresh(map);
                    SSLEEP(1);
                break;
            case KEY_BUTTON_E: // e - enemy1 $ 36
                    mvwaddch(map,(pMapdata->en1_y),(pMapdata->en1_x),' ');
                    mvwaddch(map,pl_y,pl_x,36);
                    pMapdata->en1_y=pl_y;
                    pMapdata->en1_x=pl_x;
                    mvwprintw(stats,1,13,"Enemy 2 Set     ");
                    wrefresh(stats);
                    wmove(map,pl_y,pl_x);
                    wrefresh(map);
                    SSLEEP(1);
                break;
            case KEY_BUTTON_R: // r - enemy2 & 38
                    mvwaddch(map,(pMapdata->en2_y),(pMapdata->en2_x),' ');
                    mvwaddch(map,pl_y,pl_x,38);
                    pMapdata->en2_y=pl_y;
                    pMapdata->en2_x=pl_x;
                    mvwprintw(stats,1,13,"Enemy 3 Set     ");
                    wrefresh(stats);
                    wmove(map,pl_y,pl_x);
                    wrefresh(map);
                    SSLEEP(1);
                break;
            case KEY_BUTTON_T: // t - enemy3 ! 33
                    mvwaddch(map,(pMapdata->en3_y),(pMapdata->en3_x),' ');
                    mvwaddch(map,pl_y,pl_x,33);
                    pMapdata->en3_y=pl_y;
                    pMapdata->en3_x=pl_x;
                    mvwprintw(stats,1,13,"Enemy 4 Set     ");
                    wrefresh(stats);
                    wmove(map,pl_y,pl_x);
                    wrefresh(map);
                    SSLEEP(1);
                break;
            case KEY_BUTTON_BACKSPACE: // backspace
                    mvwaddch(map,pl_y,pl_x,' ');
                    pl_x--;
                break;
            case KEY_BUTTON_SPACE:
                    mvwaddch(map,pl_y,pl_x,' ');
                    pl_x++;
                break;
            case KEY_BUTTON_ONE: 
                    help=true;
                break;
            case KEY_BUTTON_ZERO: // 0 for emergency exit in debug mode
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

        // start world calculations after this point
        wmove(map,pl_y,pl_x);
        pl_symb=mvwinch(map,pl_y,pl_x);
        mvwprintw(stats,1,1,"    y     x          symbol");
        mvwprintw(stats,1,1,"%3d y %3d x %8d symbol",pl_y,pl_x,pl_symb);
        wrefresh(stats);
        wrefresh(map);
        if(help)
        {
            showhelp();
            touchwin(map);
            help=false;
        }

        if(menupause)
        {
            menu_ret=editmenu(pMapdata);
            if(menu_ret==2)
            {
                gameloop=false;
            }
            else if(menu_ret==1)
            {
                mapreload=true;
            }
            else if(menu_ret==3)
            {
                mapsave=true;
            }
            touchwin(map);
            menupause=false;
        }
        if(mapsave)
        {
            // name=newwin(3,12,0,36);
            wmove(map,(pMapdata->st_y),(pMapdata->st_x));
            undrawedit(name);
            mvwprintw(stats,1,1,"Saving.                    ");
            wrefresh(stats);
            save_ret=savemap(map,pMapdata);
            if(save_ret==1)
            {
                goto goto_error;
            }
            mvwprintw(stats,1,1,"Map saved.                 ");
            wrefresh(stats);
            touchwin(map);
            wmove(map,pl_y,pl_x);
            wrefresh(map);
            mapsave=false;
        }
        // reload procedure - only use to reload maps on new game
        if(mapreload)
        {
            //mvwaddch(stats,1,1,' ');
            //wrefresh(stats);
            draw_ret=readmap(pMapdata,pMaparr);
            if(draw_ret==1)
            {
                goto goto_error;
            }
            touchwin(map);
            wrefresh(map);
            undraw(map,stats);
            undrawedit(name);

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
            //init game window after this point
            pl_y=pMapdata->st_y;
            pl_x=pMapdata->st_x;
            box(name,0,0);
            mvwaddstr(name,1,1,pMapdata->file_name);
            wrefresh(name);
            gameloop=true;
            box(stats,0,0);
            pl_symb=mvwinch(map,pl_y,pl_x);
            mvwprintw(stats,1,1,"    y     x          symbol");
            mvwprintw(stats,1,1,"%3d y %3d x %8d symbol",pl_y,pl_x,pl_symb);
            wrefresh(stats);
            mvwaddch(map,pl_y,pl_x,1088);
            wmove(map,pl_y,pl_x);
            wrefresh(map);
            touchwin(map);
            //
            mapreload=false;
        }
        //wait_nano(WAIT_60HZ);
    }
    while(gameloop);
    //error exit - after this point all malloc data deninitialze
    //DO NOT MALLOC something in the loop!!!!!
    goto_error: ;
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