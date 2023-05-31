#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int pipeLine(char child[], char option[]){
	int thepipe[2], newfd, pid;
	
	if(pipe(thepipe) == -1){
		perror("pipe");
		exit(1);
	}
	if((pid = fork()) == -1){
		perror("fork");
		exit(1);
	}
	if(pid > 0){
		close(thepipe[1]);
		if(dup2(thepipe[0], 0) == -1){
			perror("couldm't redirect");
			exit(1);
		}
		close(thepipe[0]);
		execl("./grep","./grep",option, NULL);
		exit(1);
	}
	close(thepipe[0]);
	if(dup2(thepipe[1],1) == -1){
		perror("couldn't redirct");
		exit(1);
	}
	close(thepipe[1]);
	execlp(child, child, NULL);

	exit(1);
}

