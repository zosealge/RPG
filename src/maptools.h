#define MY 26
#define MX 100

int readmap(struct map_data *data0,int **d)
{
    const char *map_folder="maps";
    char path[24];
    char buffer[64];
    int ch;
    int y_i;
    int x_i;
    int sy;
    int sx;
    int y;
    int x;
    FILE *mapfile;
    strncpy(path,map_folder,5);
    strncat(path,"/",2);
    strncat(path,data0->file_name,12);
    mapfile=fopen(path,"r");
    if(mapfile==NULL) return 1;
    fgets(buffer,64,mapfile);
    sscanf(buffer,"%d %d",&sy,&sx);
    data0->st_y=sy;
    data0->st_x=sx;
    //fill memory with zeros to not show garbage data
    for(y=0;y<MY;y++)
    {
        for(x=0;x<MX;x++)
        {
            d[y][x]=0;
        }
        x=0;
    }
    //map_maxy map_maxx pl_st_y pl_st_x - map file structure
    //030 030 005 005
    while(!feof(mapfile))
    {
            //chars stored as int
            //y   x   int
            //005 006 026
            //
            fgets(buffer,24,mapfile);
            sscanf(buffer,"%d %d %d",&y_i,&x_i,&ch);
            d[y_i][x_i]=ch;
    }
    fclose(mapfile);
    return 0;
}

int drawmap(WINDOW *map,int **d,struct map_data *data0)
{
    int y;
    int x;
    wrefresh(map);
    for(y=2;y<MY;y++)
    {
        for(x=2;x<MX-1;x++)
        {
             if((d[y][x])==35)  mvwaddch(map,y,x,547);
        else if((d[y][x])==115) mvwaddch(map,y,x,1395);
        else if((d[y][x])==42)  mvwaddch(map,y,x,2090);
        else if((d[y][x])==84)  mvwaddch(map,y,x,2388);
        else if((d[y][x])==37)
        {
            data0->en0_y=y;
            data0->en0_x=x;
            mvwaddch(map,y,x,37);
        }
        else if((d[y][x])==36)
        {
            data0->en1_y=y;
            data0->en1_x=x;
            mvwaddch(map,y,x,36);
        }
        else if((d[y][x])==38)
        {
            data0->en2_y=y;
            data0->en2_x=x;
            mvwaddch(map,y,x,38);
        }
        else if((d[y][x])==33)
        {
            data0->en3_y=y;
            data0->en3_x=x;
            mvwaddch(map,y,x,33);
        }
        else mvwaddch(map,y,x,' ');
        }
        x=2;
        wrefresh(map);
        wait_nano(19999999);
    }

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
    wrefresh(map);
    return 0;
}

int savemap(WINDOW *map,struct map_data *data0)
{
    const char *map_folder="maps";
    char path[24];
    int ch;
    int y_i;
    int x_i;
    FILE *mapfile;
    strncpy(path,map_folder,sizeof(&map_folder));
    strcat(path,"/");
    strcat(path,data0->file_name);
    mapfile=fopen(path,"w");
    if(mapfile==NULL)
    {
        return -1;
    }
    y_i=2;
    x_i=2;
    //sprintf(buffer,"%3d %3d",data0->st_y,data0->st_x);
    //header information
    //001 002
    //player 
    //  y   x
    //
    //map data
    //001 002 003
    //  y   x  ch
    fprintf(mapfile,"%3d %3d\n",data0->st_y,data0->st_x);
    wmove(map,y_i,x_i);
    //end of header write
    //start read/write loop
    for(y_i=2;y_i<MY;y_i++)
    {
        for(x_i=2;x_i<MX;x_i++)
        {
            ch=mvwinch(map,y_i,x_i);
            if(ch==547) ch=35;
            else if(ch==1088) ch=64;
            else if(ch==1395) ch=115;
            else if(ch==2090) ch=42;
            else if(ch==2388) ch=84;
            else if(ch<32) continue;
            //else if(ch>126) continue;
            fprintf(mapfile,"%3d %3d %3d\n",y_i,x_i,ch);
            wmove(map,y_i,x_i);
            wrefresh(map);
        }
        x_i=2;
    }
    fclose(mapfile);
    return 0;
}

void redraw(WINDOW *map)
{
    int y;
    int x;
    for(y=1,x=1;y<MY;y++)
    {
        mvwaddch(map,y,x,2388);
        wrefresh(map);
        wmove(map,y,x);
        wait_nano(5999999);
    }

    for(y=1,x=99;y<MY;y++)
    {
        mvwaddch(map,y,x,2388);
        wrefresh(map);
        wmove(map,y,x);
        wait_nano(5999999);
    }

    for(y=1,x=1;x<MX;x++)
    {
        mvwaddch(map,y,x,2388);
        wrefresh(map);
        wmove(map,y,x);
        wait_nano(1999999);
    }

    for(y=26,x=1;x<MX;x++)
    {
        mvwaddch(map,y,x,2388);
        wrefresh(map);
        wmove(map,y,x);
        wait_nano(1999999);
    }
    wrefresh(map);
}

void undraw(WINDOW *map,WINDOW *stats)
{
    int y;
    int x;
    for(y=0;y<MY+1;y++)
    {
        for(x=0;x<MX;x++)
        {
            mvwaddch(map,y,x,' ');
        }
        wrefresh(map);
        wait_nano(9999999);
        x=0;
    }
    for(int i=0;i<31;i++)
    {
        mvwaddch(stats,0,i,' ');
        mvwaddch(stats,1,i,' ');
        mvwaddch(stats,2,i,' ');
        wrefresh(stats);
        wait_nano(9999999);
    }
}

void undrawedit(WINDOW *name)
{
    for(int i=0;i<14;i++)
        {
            mvwaddch(name,0,i,' ');
            mvwaddch(name,1,i,' ');
            mvwaddch(name,2,i,' ');
            wrefresh(name);
            wait_nano(9999999);
        }
}