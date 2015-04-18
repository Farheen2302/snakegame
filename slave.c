/*  Make the necessary includes and set up the variables.  */

#include <sys/types.h>
#include <stdlib.h>
#include <curses.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include"utility_func.c"
struct snake{
    int snake_array[50][2];
    int snakelength;
    int food[2];
    int score;
    int x,y;
    int flag;
    int tempflag2;
    int minitem;
};

typedef struct snake snake;
/***********************SLAVE SNAKE*********************************************/

int main(int argc, char *argv[])
{
    int sockfd;
    int len,result;
    snake *slave,*master;
    int my_score=0,other_score=0;
    int lastfood[2];

    int ch,flaggy,foodcount,level=0;
    struct sockaddr_in address;
    char *srv_ip;
    slave=(snake *)malloc(sizeof(snake));
    master=(snake *)malloc(sizeof(snake));

    slave->snakelength=5;
    slave->x=-1;
    slave->y=0;
    slave->score=0;
    slave->flag=1;


    //snake will start from right end     
    createsnake(slave->snake_array,2);
    createsnake(master->snake_array,1);
    /*  Create a socket for the client.  */

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket, as agreed with the server.  */

    address.sin_family = AF_INET;

    if(argc==2)
        srv_ip=argv[1];
    address.sin_addr.s_addr = inet_addr(srv_ip);
    address.sin_port = htons(9734);
    len = sizeof(address);

    /*  Now connect our socket to the server's socket.  */

    result = connect(sockfd, (struct sockaddr *)&address, len);

    if(result == -1) {
        perror("oops: client2");
        endwin();
        exit(1);
    }

    /*  We can now read/write via sockfd.  */


    initscr();

    curs_set(0);

    //inisilize colors
    colors();

    //WEL COME SCREEN	
    welcome_screen();


    do
    {
	switch(level)
	{
		case 0: foodcount=5;
			master->minitem=2;
			slave->minitem=2;
			flaggy=1;
			while(flaggy)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@222
			{
			//reading slave snake attribites
			read(sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			lastfood[0]=master->food[0];
        lastfood[1]=master->food[1];

        if((lastfood[0]!=slave->food[0])||(lastfood[1]!=slave->food[1]))
            slave->tempflag2=0;

			///////////////			
			attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(slave->food);
			slave->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (slave->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (slave->score)+=10;
			    slave->tempflag2=1;
			    --(slave->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(slave->x!=0)&&(slave->y!=1))
			    {
				slave->x=0;
				slave->y=-1;
			    }
			    if((ch=='s')&&(slave->x!=0)&&(slave->y!=-1))
			    {
				slave->x=0;
				slave->y=1;
			    }
			    if((ch=='a')&&(slave->x!=1)&&(slave->y!=0))
			    {	
				slave->x=-1;
				slave->y=0;
			    }
			    if((ch=='d')&&(slave->x!=-1)&&(slave->y!=0))
			    {
				slave->x=1;
				slave->y=0;
			    }

			}


			if(master->minitem<=0 && slave->minitem<=0)
			{
				if(foodcount==0)
				{
					++level;
					flaggy=0;
				}
			}
			else
			{
			//else print karana h ki level cross hi ni kiya
				flaggy=0; //final flag for determining end of game or start of next level
			}
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=slave->score;
				other_score=master->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;

			//isime maze bhi banega
			// set by ncurses display timelimit 5min;
		case 1: foodcount=6;
			master->minitem=2;
			slave->minitem=2;
			//reading slave snake attribites
			//reading slave snake attribites
			flaggy=1;
			while(flaggy)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@222
			{
			//reading slave snake attribites
			read(sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			lastfood[0]=master->food[0];
        lastfood[1]=master->food[1];

        if((lastfood[0]!=slave->food[0])||(lastfood[1]!=slave->food[1]))
            slave->tempflag2=0;

			///////////////			
			attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(slave->food);
			slave->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (slave->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (slave->score)+=10;
			    slave->tempflag2=1;
			    --(slave->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(slave->x!=0)&&(slave->y!=1))
			    {
				slave->x=0;
				slave->y=-1;
			    }
			    if((ch=='s')&&(slave->x!=0)&&(slave->y!=-1))
			    {
				slave->x=0;
				slave->y=1;
			    }
			    if((ch=='a')&&(slave->x!=1)&&(slave->y!=0))
			    {	
				slave->x=-1;
				slave->y=0;
			    }
			    if((ch=='d')&&(slave->x!=-1)&&(slave->y!=0))
			    {
				slave->x=1;
				slave->y=0;
			    }

			}


			if(master->minitem<=0 && slave->minitem<=0)
			{
				if(foodcount==0)
				{
					++level;
					flaggy=0;
				}
			}
			else
			{
			//else print karana h ki level cross hi ni kiya
				flaggy=0; //final flag for determining end of game or start of next level
			}
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=slave->score;
				other_score=master->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;//set by ncurses display timelimit 6min;
		case 2: foodcount=7;
			master->minitem=2;
			slave->minitem=2;
			//reading slave snake attribites
			//reading slave snake attribites
			flaggy=1;
			while(flaggy)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@222
			{
			//reading slave snake attribites
			read(sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			lastfood[0]=master->food[0];
        lastfood[1]=master->food[1];

        if((lastfood[0]!=slave->food[0])||(lastfood[1]!=slave->food[1]))
            slave->tempflag2=0;

			///////////////			
			attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(slave->food);
			slave->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (slave->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (slave->score)+=10;
			    slave->tempflag2=1;
			    --(slave->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(slave->x!=0)&&(slave->y!=1))
			    {
				slave->x=0;
				slave->y=-1;
			    }
			    if((ch=='s')&&(slave->x!=0)&&(slave->y!=-1))
			    {
				slave->x=0;
				slave->y=1;
			    }
			    if((ch=='a')&&(slave->x!=1)&&(slave->y!=0))
			    {	
				slave->x=-1;
				slave->y=0;
			    }
			    if((ch=='d')&&(slave->x!=-1)&&(slave->y!=0))
			    {
				slave->x=1;
				slave->y=0;
			    }

			}


			if(master->minitem<=0 && slave->minitem<=0)
			{
				if(foodcount==0)
				{
					++level;
					flaggy=0;
				}
			}
			else
			{
			//else print karana h ki level cross hi ni kiya
				flaggy=0; //final flag for determining end of game or start of next level
			}
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=slave->score;
				other_score=master->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;
		case 3: foodcount=8;
			//reading slave snake attribitesfoodcount=5;
			master->minitem=2;
			slave->minitem=2;
			flaggy=1;
			while(flaggy)//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@222
			{
			//reading slave snake attribites
			read(sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			lastfood[0]=master->food[0];
        lastfood[1]=master->food[1];

        if((lastfood[0]!=slave->food[0])||(lastfood[1]!=slave->food[1]))
            slave->tempflag2=0;

			///////////////			
			attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(slave->food);
			slave->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (slave->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (slave->score)+=10;
			    slave->tempflag2=1;
			    --(slave->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(slave->x!=0)&&(slave->y!=1))
			    {
				slave->x=0;
				slave->y=-1;
			    }
			    if((ch=='s')&&(slave->x!=0)&&(slave->y!=-1))
			    {
				slave->x=0;
				slave->y=1;
			    }
			    if((ch=='a')&&(slave->x!=1)&&(slave->y!=0))
			    {	
				slave->x=-1;
				slave->y=0;
			    }
			    if((ch=='d')&&(slave->x!=-1)&&(slave->y!=0))
			    {
				slave->x=1;
				slave->y=0;
			    }

			}


			if(master->minitem<=0 && slave->minitem<=0)
			{
				if(foodcount==0)
				{
					++level;
					flaggy=0;
				}
			}
			else
			{
			//else print karana h ki level cross hi ni kiya
				flaggy=0; //final flag for determining end of game or start of next level
			}
			}
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=slave->score;
				other_score=master->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;
	}
}while(flaggy);
           
    
    erase();
    move(LINES/2,COLS/2-6);
    sleep(1);
    if(other_score>my_score)
    {
        printw("MASTER WINS");
    }
    else
        if(my_score != 0)
        {
            printw("SLAVE WINS");
        }
        else
            printw("MATCH DRAWN");
    refresh();
    sleep(10);

    close_keyboard();
    endwin();
    close(sockfd);
    return(0);

}

