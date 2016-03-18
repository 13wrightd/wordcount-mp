#include <stdio.h>
//#include <iostream>
//#include <string>
// Pipe is cleared after read
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 

main(int argc, char *argv[]){

	int fd[2];
	int child_pid;
	char buf[30];

	if(pipe(fd) == -1){
		perror("failed to setup pipe.");
		return 1;
	}

	child_pid = fork();
	if(child_pid == -1){
		perror("failed to fork.");
		return 1;
	}
	else if(child_pid == 0){	//child
		write(fd[1], "test", 5);
		close(fd[1]);
	}
	else if(child_pid > 0){		//parent
		read(fd[0], buf, 5);
		printf("%s\n", buf);
		//read(fd[0], buf, 5);
		printf("%s\n", buf);
	}


	return 0;
}