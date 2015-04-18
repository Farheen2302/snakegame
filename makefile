all: master slave

master: master.c utility_func.c
	cc -o master master.c -lncurses

slave: slave.c utility_func.c
	cc -o slave slave.c -lncurses
	
clean:
	  rm -rf master slave
