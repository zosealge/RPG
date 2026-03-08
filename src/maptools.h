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

#define MY 26
#define MX 100

int readmap(struct map_data *data0,int **d)
{
    struct map_header
    {
        uint8_t pl_y_pos;
        uint8_t pl_x_pos;
        uint8_t en0_y_pos;
        uint8_t en0_x_pos;
        uint8_t en1_y_pos;
        uint8_t en1_x_pos;
        uint8_t en2_y_pos;
        uint8_t en2_x_pos;
        uint8_t en3_y_pos;
        uint8_t en3_x_pos;
    };
    struct map_board
    {
        uint8_t y;
        uint8_t x;
        uint8_t tile;
    };
    struct map_header *mh=malloc(sizeof(struct map_header));
    if(mh==NULL)
    {
        return -2;
    }
    struct map_board *mb=malloc(sizeof(struct map_board));
    if(mb==NULL)
    {
        return -2;
    }
    const char *map_folder="maps";
    const int RW_SIZE=1;
    char path[24];
    int y;
    int x;
    int rc;
    FILE *mapfile;
    strncpy(path,map_folder,5);
    strncat(path,"/",2);
    strncat(path,data0->file_name,12);
    mapfile=fopen(path,"rb");
    if(mapfile==NULL) return 1;
    rc=fread(mh,sizeof(struct map_header),RW_SIZE,mapfile);
    if(rc<0)
    {
        return -2;
    }
    data0->st_y=mh->pl_y_pos;
    data0->st_x=mh->pl_x_pos;
    //fill memory with zeros to not show garbage data
    for(y=0;y<MY;y++)
    {
        for(x=0;x<MX;x++)
        {
            d[y][x]=0;
        }
        x=0;
    }
    while(!feof(mapfile))
    {
            rc=fread(mb,sizeof(struct map_board),RW_SIZE,mapfile);
            d[mb->y][mb->x]=mb->tile;
    }
    fclose(mapfile);
    free(mh);
    free(mb);
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
    wrefresh(map);
    return 0;
}

int savemap(WINDOW *map,struct map_data *data0)
{
    struct map_header
    {
        uint8_t pl_y_pos;
        uint8_t pl_x_pos;
        uint8_t en0_y_pos;
        uint8_t en0_x_pos;
        uint8_t en1_y_pos;
        uint8_t en1_x_pos;
        uint8_t en2_y_pos;
        uint8_t en2_x_pos;
        uint8_t en3_y_pos;
        uint8_t en3_x_pos;
    };
    struct map_board
    {
        uint8_t y;
        uint8_t x;
        uint8_t tile;
    };
    struct map_header *mh=malloc(sizeof(struct map_header));
    if(mh==NULL)
    {
        return -2;
    }
    struct map_board *mb=malloc(sizeof(struct map_board));
    if(mb==NULL)
    {
        return -2;
    }
    const char *map_folder="maps";
    const int RW_SIZE=1;
    char path[24];
    int ch;
    int y_i;
    int x_i;
    FILE *mapfile;
    strncpy(path,map_folder,sizeof(&map_folder));
    strcat(path,"/");
    strcat(path,data0->file_name);
    mapfile=fopen(path,"wb");
    if(mapfile==NULL)
    {
        return -1;
    }
    y_i=2;
    x_i=2;
    memset(mh,0,sizeof(struct map_header));
    memset(mb,0,sizeof(struct map_board));
    mh->pl_y_pos=data0->st_y;
    mh->pl_x_pos=data0->st_x;
    fwrite(mh,sizeof(struct map_header),RW_SIZE,mapfile);
    wmove(map,y_i,x_i);
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
            mb->y=y_i;
            mb->x=x_i;
            mb->tile=ch;
            fwrite(mb,sizeof(struct map_board),RW_SIZE,mapfile);
            wmove(map,y_i,x_i);
            wrefresh(map);
        }
        x_i=2;
    }
    fclose(mapfile);
    free(mh);
    free(mb);
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