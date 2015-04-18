#include<termios.h>
#include<term.h>


static struct termios initial_settings,new_settings;
static int peek_character=-1;


int kbhit()
{
    char ch;
    int nread;
    if(peek_character!=-1)
        return 1;
    new_settings.c_cc[VMIN]=0;
    tcsetattr(0,TCSANOW,&new_settings);
    nread=read(0,&ch,1);
    new_settings.c_cc[VMIN]=1;
    tcsetattr(0,TCSANOW,&new_settings);
    if(nread==1)
    {
        peek_character=ch;
        return 1;
    }
    return 0;
}

void init_keyboard()
{
    tcgetattr(0,&initial_settings);
    new_settings=initial_settings;
    new_settings.c_lflag &=~ICANON;
    new_settings.c_lflag &=~ECHO;
    new_settings.c_lflag &=~ISIG;
    new_settings.c_cc[VMIN]=1;
    new_settings.c_cc[VTIME]=0;
    tcsetattr(0,TCSANOW,&new_settings);
}

void close_keyboard()
{
    tcsetattr(0,TCSANOW,&initial_settings);
}

int readch()
{
    char ch;
    if(peek_character!=-1)
    {
        ch=peek_character;
        peek_character=-1;
        return ch;
    }
    read(0,&ch,1);
    return ch;
}

void colors()
{

    if(!has_colors())
    {
        printw("no color");
        exit(0);
    }
    if(start_color()!=OK)
    {
        printw("cannot start colour");
        exit(0);
    }

    init_pair(1,COLOR_BLUE,COLOR_BLUE);
    init_pair(2,COLOR_RED,COLOR_RED);
    init_pair(3,COLOR_GREEN,COLOR_GREEN);
}


void welcome_screen()
{
    move(4,COLS/2-5);
    printw("SNAKE GAME");
    move(6,COLS/2-3);
    printw("made by");
    move(8,COLS/2-16);

    printw("12CSS-21,56,57,68,73");
    move(10,COLS/2-9);
    printw("ENJOY UNLIMITED");
    move(12,COLS/2-4);
    printw("CONTROLS");
    move(13,COLS/2-16);
    printw("PLAYER 1                PLAYER2");
    move(14,COLS/2-16);
    printw("w = up                  i = up");
    move(15,COLS/2-16);
    printw("s = down                k = down");
    move(16,COLS/2-16);
    printw("a = left                j = left");
    move(17,COLS/2-16);
    printw("d = right               l = right");
    move(20,COLS/2-16);
    printw("press any key to get started....");
    getch();
}


int gamenotend(int x,int y,int snakelength,int snake_array[50][2])
{
    int i;
    if((x==0)||(y==0)||(x>=COLS-1)||(y>=5*LINES/6))

        return 0;
    if(snakelength>5)
    {
        for(i=1;i<snakelength;i++)
            if((snake_array[i][0]==y)&&(snake_array[i][1]==x))
                return 0;
    }
    //not end
    return 1;

}





void movesnake(int x,int y,int snakelength,int snake_array[100][2])
{
    int i;

    for(i=snakelength-1;i>=0;i--)
    {
        if(i==0)
        {
            snake_array[i][0]+=y;
            snake_array[i][1]+=x;
        }
        else
        {
            snake_array[i][0]=snake_array[i-1][0];
            snake_array[i][1]=snake_array[i-1][1];
        }

        move(snake_array[i][0],snake_array[i][1]);
        if(i!=0)
            printw("#");
        else
            printw("@");

    }

    refresh();

}



void foodprint(int *food)
{
    attrset(COLOR_PAIR(3)|A_BOLD);	
    move(food[1],food[0]);
    printw("*");
    attroff(COLOR_PAIR(3)|A_BOLD);
    refresh();
    return ;
}



int foodeaten(int *food,int x,int y)
{
    if((y==*(food+1))&&(x==*food))
    {
        return 0;
    }
    else
        return 1;
}





void createsnake(int snake_array[][2],int j)
{
    int i;

    for(i=4;i>=0;i--)
    {
        snake_array[i][0]=5;
        if(j==1)
            snake_array[i][1]=i+5;
        else
            snake_array[i][1]=100-10+i;
    }

}


void printscore(int master,int slave)
{

    {	
        move((5*LINES/6)+2,(COLS/4)-4);  //printing score
        printw("MASTER SCORE:%d",master);
    }

    {
        move((5*LINES/6)+2,(3*COLS/4)-4);  //printing score
        printw("SLAVE SCORE:%d",slave);

    }
    refresh();
}
int checkcollision(int snake_array1[100][2],int snakelength1,int snake_array2[100][2],int snakelength2)
{

    int i;
    for(i=0;i<snakelength1;i++)

        if((snake_array2[0][0]==snake_array1[i][0])&&(snake_array2[0][1]==snake_array1[i][1]))
            return 0;


    for(i=0;i<snakelength2;i++)

        if((snake_array1[0][0]==snake_array2[i][0])&&(snake_array1[0][1]==snake_array2[i][1]))
            return 0;

    //no collision
    return 1;
}
