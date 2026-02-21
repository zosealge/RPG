void mana_boom_anim(WINDOW *screen,int y, int x);

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

void mana_boom_anim(WINDOW *screen,int y, int x)
{
    const long WAIT_BOOM=96666666;

    wattron(screen,COLOR_PAIR(12));
    mvwaddstr(screen,y-1,x-1," # ");
    mvwaddstr(screen,y,x-1,  "###");
    mvwaddstr(screen,y+1,x-1," # ");
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
    mvwaddstr(screen,y-1,x-1,"   ");
    mvwaddstr(screen,y,x-1,  " . ");
    mvwaddstr(screen,y+1,x-1,"   ");
    wattroff(screen,COLOR_PAIR(20));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);

    wattron(screen,COLOR_PAIR(4));
    mvwaddch(screen,y,x,'_');
    wattroff(screen,COLOR_PAIR(4));
    wrefresh(screen);
    wait_nano(WAIT_BOOM);
}

// Returns:
// 0 - no action
// 1 - hit obstacle
// 2 - hit mana
// 10 - hit enem0
// 11 - hit enem1
// 12 - hit enem2
// 13 - hit enem3

int fireballaction(WINDOW *m,struct player *p)
{
    int symb;
    mvwaddch(m,p->f_ball_y,p->f_ball_x,' ');
         if(p->fireball_dir==1) //left
    {
        p->f_ball_x-=1;
        symb=mvwinch(m,p->f_ball_y,p->f_ball_x);
    }
    else if(p->fireball_dir==2) //up
    {
        p->f_ball_y-=1;
        symb=mvwinch(m,p->f_ball_y,p->f_ball_x);
    }
    else if(p->fireball_dir==3) //down
    {
        p->f_ball_y+=1;
        symb=mvwinch(m,p->f_ball_y,p->f_ball_x);
    }
    else if(p->fireball_dir==4) //right
    {
        p->f_ball_x+=1;
        symb=mvwinch(m,p->f_ball_y,p->f_ball_x);
    }
    if(symb==32)
    {   
             if(p->fireball_dir==1) mvwaddch(m,p->f_ball_y,p->f_ball_x,'-');
        else if(p->fireball_dir==2) mvwaddch(m,p->f_ball_y,p->f_ball_x,'|');
        else if(p->fireball_dir==3) mvwaddch(m,p->f_ball_y,p->f_ball_x,'|');
        else if(p->fireball_dir==4) mvwaddch(m,p->f_ball_y,p->f_ball_x,'-');
        return 0;
    }
    else if(symb==547)
    {
        mvwaddch(m,p->f_ball_y,p->f_ball_x,' ');
        return 1;
    }
    else if(symb==1395)
    {
        //mvwaddch(m,p->f_ball_y,p->f_ball_x,' ');
        return 1;
    }
    else if(symb==2388)
    {
        //mvwaddch(m,p->f_ball_y,p->f_ball_x,' ');
        return 1;
    }
    else if(symb==2090)
    {
        mana_boom_anim(m,p->f_ball_y,p->f_ball_x);
        return 1;
    }
    else if(symb==37)
    {
        return 10;
    }
    else if(symb==36)
    {
        return 11;
    }
    else if(symb==38)
    {
        return 12;
    }
    else if(symb==33)
    {
        return 13;
    }
    return 0;
}

void play0_init(struct player *p)
{ 
    p->symb='@';
    p->health=100;
    p->mana=0;
    p->dir=1;
    p->fireball_dir=1;
}