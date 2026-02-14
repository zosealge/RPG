int readmap(WINDOW *mmenu,void *p,int **d,char *mapname);
int drawmap(WINDOW *map,void *p,int **d);

int readmap(WINDOW *mmenu,void *p,int **d,char *mapname)
{
    const char *map_folder="maps";
    struct map_data *data0=p;
    char path[24];
    char buffer[64];
    int ch;
    int y_i;
    int x_i;
    int my;
    int mx;
    int sy;
    int sx;
    int i=0;
    bool readmap;
    FILE *mapfile;
    strcpy(path,map_folder);
    strcat(path,"/");
    strcat(path,mapname);
    strcpy(data0->file_name,mapname);
    mapfile=fopen(path,"r");
    if(mapfile==NULL) return -1;
    readmap=true;
    fgets(buffer,64,mapfile);
    sscanf(buffer,"%d %d %d %d",&my,&mx,&sy,&sx);
    data0->maxy=my;
    data0->maxx=mx;
    data0->st_y=sy;
    data0->st_x=sx;
    //fill memory with zeros to not show garbage data
    for(int x=0;x<mx;x++)
    {
        for(int y=0;y<my;y++)
        {
            d[y][x]=0;
        }
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

int drawmap(WINDOW *map,void *p,int **d)
{
    struct map_data *data0=p;
    int my;
    int mx;
    int y;
    int x;
    my=data0->maxy;
    mx=data0->maxx;
    bool drawmap=false;
    wclear(map);
    box(map,0,0);
    wrefresh(map);
    for(x=0;x<mx;x++)
    {
        for(y=0;y<my;y++)
        {
            if((d[y][x])==35)
            {
                wattron(map,COLOR_PAIR(2));
                mvwaddch(map,y,x,d[y][x]);
                wattroff(map,COLOR_PAIR(2));
            }
            else if((d[y][x])==42)
            {
                wattron(map,COLOR_PAIR(8));
                mvwaddch(map,y,x,d[y][x]);
                wattroff(map,COLOR_PAIR(8));
            }
        }
    }
    wrefresh(map);
    while(drawmap);
    return 0;
}