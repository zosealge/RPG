//int enemymove(WINDOW *screen);
//{'%',100,0,20,20,false};
//symb,hea,a,y ,x ,dead
int enemymove(WINDOW *screen,struct enemy *en)
{
    const long WAIT_BOOM=96666666;
    int symb;
    //1 left
    //2 up
    //3 down
    //4 right
    if((en->is_dead)==false)
    {
    //important ascii codes:
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
        4194424 - side panels box
        4194417 - top and bottom panels box
    */
        switch((rand()%50)+1)
        {   //cases 1-10 movement 30-40 atack action
            case 1: //left
            en->dir=1;
            en->e_symb=mvwinch(screen,en->ey,en->ex-1);
            if((en->e_symb)==4194424) break;
            else if((en->e_symb)==547) break;
            else if((en->e_symb)==2388) break;
            else if((en->e_symb)==1395) break;
            else
            {
                mvwaddch(screen,en->ey,en->ex,' ');
                en->ex=(en->ex)-1;
                mvwaddch(screen,en->ey,en->ex,en->symb);
            }
            break;
            case 2: //up
            en->dir=2;
            en->e_symb=mvwinch(screen,en->ey-1,en->ex);
            if((en->e_symb)==4194417) break;
            else if((en->e_symb)==547) break;
            else if((en->e_symb)==2388) break;
            else if((en->e_symb)==1395) break;
            else
            {
                mvwaddch(screen,en->ey,en->ex,' ');
                en->ey=(en->ey)-1;
                mvwaddch(screen,en->ey,en->ex,en->symb);
            }
            break;
            case 3: //down
            en->dir=3;
            en->e_symb=mvwinch(screen,en->ey+1,en->ex);
            if((en->e_symb)==4194417) break;
            else if((en->e_symb)==547) break;
            else if((en->e_symb)==2388) break;
            else if((en->e_symb)==1395) break;
            else
            {
                mvwaddch(screen,en->ey,en->ex,' ');
                en->ey=(en->ey)+1;
                mvwaddch(screen,en->ey,en->ex,en->symb);
            }
            break;
            case 4: //right
            en->dir=4;
            en->e_symb=mvwinch(screen,en->ey,en->ex+1);
            if((en->e_symb)==4194424) break;
            else if((en->e_symb)==547) break;
            else if((en->e_symb)==2388) break;
            else if((en->e_symb)==1395) break;
            else
            {
                mvwaddch(screen,en->ey,en->ex,' ');
                en->ex=(en->ex)+1;
                mvwaddch(screen,en->ey,en->ex,en->symb);
            }
            break;
            case 30: //fire
                if((en->is_shooting)) break;
                en->is_shooting=true;
                en->fireball_dir=en->dir;
                en->fireball_y=en->ey;
                en->fireball_x=en->ex;
                //mvwaddstr(screen,2,2,"bam!");
            break;
            default:
            //
            //shooting loop
            //
            //
            //
            if((en->is_shooting)==true)
            {
                switch((en->fireball_dir))
                {
                    case 1://left x-
                    en->fireball_x-=1;
                    symb=mvwinch(screen,en->fireball_y,en->fireball_x-1);
                    if(symb==4194424)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==547)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1395)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==2388)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1088)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                        en->is_hit=true;
                    }
                    else
                    {
                        if((en->fireball_x+1)!=(en->ex))
                        {
                        mvwaddch(screen,en->fireball_y,en->fireball_x+1,' ');
                        }
                        wrefresh(screen);
                        mvwaddch(screen,en->fireball_y,en->fireball_x,'-');
                    }
                    break;

                    case 2://up y-
                    en->fireball_y-=1;
                    symb=mvwinch(screen,en->fireball_y,en->fireball_x);
                    if(symb==4194417)
                    {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==547)
                    {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1395)
                    {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==2388)
                    {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1088)
                    {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                        en->is_hit=true;
                    }
                    else
                    {
                        if((en->fireball_y+1)!=(en->ey))
                        {
                        mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        }
                        //mvwaddch(screen,en->fireball_y+1,en->fireball_x,' ');
                        wrefresh(screen);
                        mvwaddch(screen,en->fireball_y,en->fireball_x,'|');
                    }
                    break;

                    case 3://down y+
                    en->fireball_y+=1;
                    symb=mvwinch(screen,en->fireball_y,en->fireball_x);
                    if(symb==4194417)
                    {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==547)
                    {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1395)
                    {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==2388)
                    {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1088)
                    {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                        en->is_hit=true;
                    }
                    else
                    {
                        if((en->fireball_y-1)!=(en->ey))
                        {
                        mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        }
                        //mvwaddch(screen,en->fireball_y-1,en->fireball_x,' ');
                        wrefresh(screen);
                        mvwaddch(screen,en->fireball_y,en->fireball_x,'|');
                    }
                    break;

                    case 4://right x+
                    en->fireball_x+=1;
                    symb=mvwinch(screen,en->fireball_y,en->fireball_x);
                    if(symb==4194424)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==547)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1395)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==2388)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                    }
                    else if(symb==1088)
                    {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        en->fireball_dir=en->dir;
                        en->is_shooting=false;
                        en->is_hit=true;
                    }
                    else
                    {
                        if((en->fireball_x-1)!=(en->ex))
                        {
                        mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        }
                        //mvwaddch(screen,en->fireball_y,en->fireball_x-1,' ');
                        wrefresh(screen);
                        mvwaddch(screen,en->fireball_y,en->fireball_x,'-');
                    }
                    break;
                }
            
        }
        }
    }
    else
    {
            wattron(screen,COLOR_PAIR(13));
            mvwaddch(screen,en->ey,en->ex,'*');
            wattroff(screen,COLOR_PAIR(13));
            wrefresh(screen);
            wait_nano(WAIT_BOOM);
            wattron(screen,COLOR_PAIR(12));
            mvwaddch(screen,en->ey,en->ex,'X');
            wattroff(screen,COLOR_PAIR(12));
            wrefresh(screen);
            wait_nano(WAIT_BOOM);
            wattron(screen,COLOR_PAIR(16));
            mvwaddch(screen,en->ey,en->ex,'#');
            wattroff(screen,COLOR_PAIR(16));
            wrefresh(screen);
            wait_nano(WAIT_BOOM);
            wattron(screen,COLOR_PAIR(17));
            mvwaddch(screen,en->ey,en->ex,'o');
            wattroff(screen,COLOR_PAIR(17));
            wrefresh(screen);
            wait_nano(WAIT_BOOM);
            wattron(screen,COLOR_PAIR(20));
            mvwaddch(screen,en->ey,en->ex,'_');
            wattroff(screen,COLOR_PAIR(20));
            if((en->is_shooting)==true)
            {
                wattron(screen,COLOR_PAIR(13));
                mvwaddch(screen,en->fireball_y,en->fireball_x,'*');
                wattroff(screen,COLOR_PAIR(13));
                wrefresh(screen);
                wait_nano(WAIT_BOOM);
                wattron(screen,COLOR_PAIR(20));
                mvwaddch(screen,en->fireball_y,en->fireball_x,'.');
                wattroff(screen,COLOR_PAIR(20));
                en->is_shooting=false;
            }
            return 1;
    }
    {
    return 0;
    }
}

void enemyinit(struct enemy *en,int n)
{
    if(n==0)
    {
        en->symb='%';
        en->health=100;
        en->dir=1;
        en->ey=20;
        en->ex=20;
        en->is_dead=false;
        en->is_shooting=false;
    }
    else if(n==1)
    {
        en->symb='K';
        en->health=100;
        en->dir=1;
        en->ey=25;
        en->ex=27;
        en->is_dead=false;
        en->is_shooting=false;
    }
}