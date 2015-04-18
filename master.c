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
#include <time.h>
#include"utility_func.c"

struct snake{
    int snake_array[50][2];		//
    int snakelength;			// length of snake
    int food[2];			// x and y pos of food 
    int score;				// score of snake
    int x,y;				//pos of snake
    int flag;				//
    int tempflag2;			//
    int minitem;
};			

typedef struct snake snake;		//
int main()
{
    int master_sockfd, slave_sockfd;	//for socket
    int master_len, slave_len;		//for socket
    snake *master,*slave;		//made master and slave snake pointer
    int ch;				//num replace
    int i,foodcount,flaggy;			
    int tempflag1;
    int my_score=0,other_score=0;
	int level=0;	
    struct sockaddr_in master_address;	//for socket
    struct sockaddr_in slave_address;	//for socket

    master=(snake *)malloc(sizeof(snake));
    slave=(snake *)malloc(sizeof(snake));

    master->snakelength=5;
    master->x=1;
    master->y=0;
    master->score=0;
    master->flag=1;		
    master->tempflag2=1;

    createsnake(master->snake_array,1);   //ncurses
    createsnake(slave->snake_array,2);    //ncurses4
	
	   /*  Create an unnamed socket for the server.  */

    master_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    /*  Name the socket.  */

    master_address.sin_family = AF_INET;
    master_address.sin_addr.s_addr =  htonl(INADDR_ANY); 
    master_address.sin_port = htons(9734);
    master_len = sizeof(master_address);
    bind(master_sockfd, (struct sockaddr *)&master_address, master_len);

    /*  Create a connection queue and wait for clients.  */

	
		listen(master_sockfd, 5);


    /*  Accept a connection.  */

    slave_len = sizeof(slave_address);
    slave_sockfd = accept(master_sockfd, 
            (struct sockaddr *)&slave_address, &slave_len);
	if(slave_sockfd==-1)
	{
		printf("bagvbkujfbvgkie");
	}

    initscr();

    curs_set(0);

    //inisilize colors
    colors();

    //WEL COME SCREEN	
    welcome_screen();

/////main work starts///////////////

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
			read(slave_sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(slave_sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			master->tempflag2=1;
			slave->tempflag2=1;
			while((master->tempflag2==1)||(slave->tempflag2==1)||foodcount!=0)                  //food generation
			{
			    srand(time(0));
			    master->food[0]=(rand()%COLS-1);
			    master->food[1]=(rand()%((5*LINES/6)-1));

			    if(master->food[1]<=0||master->food[0]<=0)
			    {
				master->tempflag2=1;
				slave->tempflag2=1;
			    }
			    else
			    {

				--foodcount;				
				for(i=0;i<master->snakelength;i++)
				{
				    if(master->snake_array[i][0]==master->food[1]&&master->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(master->minitem);
				        break;
				    }
				}
				for(i=0;i<slave->snakelength;i++)
				{
				    if(slave->snake_array[i][0]==master->food[1]&&slave->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(slave->minitem);
				        break;
				    }
				}

				if(tempflag1==1)
				{
				    master->tempflag2=1;
				    slave->tempflag2=1;
				
				}
				else
				{
				    master->tempflag2=0;
				    slave->tempflag2=0;
				}
			    }

			}
					attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(master->food);
			master->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (master->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (master->score)+=10;
			    master->tempflag2=1;
			    --(master->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(master->x!=0)&&(master->y!=1))
			    {
				master->x=0;
				master->y=-1;
			    }
			    if((ch=='s')&&(master->x!=0)&&(master->y!=-1))
			    {
				master->x=0;
				master->y=1;
			    }
			    if((ch=='a')&&(master->x!=1)&&(master->y!=0))
			    {	
				master->x=-1;
				master->y=0;
			    }
			    if((ch=='d')&&(master->x!=-1)&&(master->y!=0))
			    {
				master->x=1;
				master->y=0;
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
			my_score=master->score;
				other_score=slave->score;

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
			read(slave_sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(slave_sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			master->tempflag2=1;
			slave->tempflag2=1;
			while((master->tempflag2==1)||(slave->tempflag2==1)||foodcount!=0)                  //food generation
			{
			    srand(time(0));
			    master->food[0]=(rand()%COLS-1);
			    master->food[1]=(rand()%((5*LINES/6)-1));

			    if(master->food[1]<=0||master->food[0]<=0)
			    {
				master->tempflag2=1;
				slave->tempflag2=1;
			    }
			    else
			    {

				--foodcount;				
				for(i=0;i<master->snakelength;i++)
				{
				    if(master->snake_array[i][0]==master->food[1]&&master->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(master->minitem);
				        break;
				    }
				}
				for(i=0;i<slave->snakelength;i++)
				{
				    if(slave->snake_array[i][0]==master->food[1]&&slave->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(slave->minitem);
				        break;
				    }
				}

				if(tempflag1==1)
				{
				    master->tempflag2=1;
				    slave->tempflag2=1;
				
				}
				else
				{
				    master->tempflag2=0;
				    slave->tempflag2=0;
				}
			    }

			}
					attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(master->food);
			master->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (master->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (master->score)+=10;
			    master->tempflag2=1;
			    --(master->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(master->x!=0)&&(master->y!=1))
			    {
				master->x=0;
				master->y=-1;
			    }
			    if((ch=='s')&&(master->x!=0)&&(master->y!=-1))
			    {
				master->x=0;
				master->y=1;
			    }
			    if((ch=='a')&&(master->x!=1)&&(master->y!=0))
			    {	
				master->x=-1;
				master->y=0;
			    }
			    if((ch=='d')&&(master->x!=-1)&&(master->y!=0))
			    {
				master->x=1;
				master->y=0;
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
			
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=master->score;
				other_score=slave->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;
			//set by ncurses display timelimit 6min;
		case 2: foodcount=7;
			master->minitem=2;
			slave->minitem=2;
			//reading slave snake attribites
			//reading slave snake attribites
			read(slave_sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(slave_sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			master->tempflag2=1;
			slave->tempflag2=1;
			while((master->tempflag2==1)||(slave->tempflag2==1)||foodcount!=0)                  //food generation
			{
			    srand(time(0));
			    master->food[0]=(rand()%COLS-1);
			    master->food[1]=(rand()%((5*LINES/6)-1));

			    if(master->food[1]<=0||master->food[0]<=0)
			    {
				master->tempflag2=1;
				slave->tempflag2=1;
			    }
			    else
			    {

				--foodcount;				
				for(i=0;i<master->snakelength;i++)
				{
				    if(master->snake_array[i][0]==master->food[1]&&master->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(master->minitem);
				        break;
				    }
				}
				for(i=0;i<slave->snakelength;i++)
				{
				    if(slave->snake_array[i][0]==master->food[1]&&slave->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(slave->minitem);
				        break;
				    }
				}

				if(tempflag1==1)
				{
				    master->tempflag2=1;
				    slave->tempflag2=1;
				
				}
				else
				{
				    master->tempflag2=0;
				    slave->tempflag2=0;
				}
			    }

			}
					attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(master->food);
			master->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (master->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (master->score)+=10;
			    master->tempflag2=1;
			    --(master->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(master->x!=0)&&(master->y!=1))
			    {
				master->x=0;
				master->y=-1;
			    }
			    if((ch=='s')&&(master->x!=0)&&(master->y!=-1))
			    {
				master->x=0;
				master->y=1;
			    }
			    if((ch=='a')&&(master->x!=1)&&(master->y!=0))
			    {	
				master->x=-1;
				master->y=0;
			    }
			    if((ch=='d')&&(master->x!=-1)&&(master->y!=0))
			    {
				master->x=1;
				master->y=0;
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
			
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=master->score;
				other_score=slave->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;
		case 3: foodcount=8;
			master->minitem=2;
			slave->minitem=2;
			//reading slave snake attribites
			read(slave_sockfd, slave, sizeof(snake));

			//writing own(master) snake attributes

			write(slave_sockfd, master, sizeof(snake));

			//	boundary wall creation

			border(ACS_VLINE,ACS_VLINE,ACS_HLINE,ACS_HLINE,ACS_ULCORNER,ACS_URCORNER,ACS_LLCORNER,ACS_LLCORNER);
			move(5*LINES/6,0);
			hline(ACS_HLINE,COLS);

			init_keyboard();
			master->tempflag2=1;
			slave->tempflag2=1;
			while((master->tempflag2==1)||(slave->tempflag2==1)||foodcount!=0)                  //food generation
			{
			    srand(time(0));
			    master->food[0]=(rand()%COLS-1);
			    master->food[1]=(rand()%((5*LINES/6)-1));

			    if(master->food[1]<=0||master->food[0]<=0)
			    {
				master->tempflag2=1;
				slave->tempflag2=1;
			    }
			    else
			    {

				--foodcount;				
				for(i=0;i<master->snakelength;i++)
				{
				    if(master->snake_array[i][0]==master->food[1]&&master->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(master->minitem);
				        break;
				    }
				}
				for(i=0;i<slave->snakelength;i++)
				{
				    if(slave->snake_array[i][0]==master->food[1]&&slave->snake_array[i][1]==master->food[0])
				    {
				        tempflag1=1;
					--(slave->minitem);
				        break;
				    }
				}

				if(tempflag1==1)
				{
				    master->tempflag2=1;
				    slave->tempflag2=1;
				
				}
				else
				{
				    master->tempflag2=0;
				    slave->tempflag2=0;
				}
			    }

			}
					attrset(COLOR_PAIR(1)|A_BOLD);
			movesnake(master->x,master->y,master->snakelength,master->snake_array);
			attroff(COLOR_PAIR(1)|A_BOLD);

			attrset(COLOR_PAIR(2)|A_BOLD);
			movesnake(slave->x,slave->y,slave->snakelength,slave->snake_array);
			attroff(COLOR_PAIR(2)|A_BOLD);


			foodprint(master->food);
			master->flag=checkcollision(master->snake_array,master->snakelength,slave->snake_array,slave->snakelength);
			//collision function change karna h........
			if((foodeaten(master->food,master->snake_array[0][1],master->snake_array[0][0])==0))
			{
			    (master->snakelength)++;  //snakelength badh rhi h...already implemented feature
			    (master->score)+=10;
			    master->tempflag2=1;
			    --(master->minitem);
			}


			//	reading direction keys from keyboard

			if(kbhit())
			{
			    ch=readch();
			    if((ch=='w')&&(master->x!=0)&&(master->y!=1))
			    {
				master->x=0;
				master->y=-1;
			    }
			    if((ch=='s')&&(master->x!=0)&&(master->y!=-1))
			    {
				master->x=0;
				master->y=1;
			    }
			    if((ch=='a')&&(master->x!=1)&&(master->y!=0))
			    {	
				master->x=-1;
				master->y=0;
			    }
			    if((ch=='d')&&(master->x!=-1)&&(master->y!=0))
			    {
				master->x=1;
				master->y=0;
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
			
			//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22
			my_score=master->score;
				other_score=slave->score;

				//live score
				printscore(master->score,slave->score);
				usleep(100000);
				erase();
			break;
	}
}while(flaggy);//kaafi conditions iski wali hatayi h coz they were related to collision
erase();
    move(LINES/2,COLS/2-6);
    sleep(1);

    if(my_score> other_score)
    {
        printw("MASTER WINS");
    }
    else
        if(other_score != 0)
        {
            printw("SLAVE WINS");
        }
        else
            printw("MATCH DRAWN");
    refresh();
    sleep(10);

    close_keyboard();
    endwin();
    close(slave_sockfd);
    return(0);

}
