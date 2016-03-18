// Daniel Wright	diw5233@psu.edu
// Evan Gutman		elg5195@psu.edu


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

//Node structure for linked list
struct word_t {
	char *word;
	int count;
	struct word_t *next;
};

main(int argc, char *argv[]){

	FILE *fp;					//pointer to file
	fp = fopen(argv[1], "r");	//opens the file
	pid_t child_pid;			//store the child pid/determine what process
	int i, n = atoi(argv[2]);	//i is a counter, n is the number of processes
	int fileSize;				//stores size of the file
	int startpoint;				//start point for reading
	int endpoint;				//end point for reading
	int startpointParent;		//stores the parents start point
	int endpointParent;			//stores the parents end point
	char c;						//for reading the file
	int **fd = (int **)malloc((n-1) * sizeof(int *));	//dynamic 2d array using points
														//for file descriptors
	int childNum;				//used to access each pipe

	for(i = 0; i < n-1; i++){	//make each row have 2 columns for file descriptors
		fd[i] = (int *)malloc(2 * sizeof(int));
	}

	startpoint = ftell(fp);		//gets the startpoint (beginning of file)

	fseek(fp, 0, SEEK_END);		//goes through to the end of the file
	fileSize = ftell(fp);		//gets the last position in file (the length/size)

	fseek(fp, 0, SEEK_SET);		//go back to the beginning of the file

	endpoint = fileSize/n;		//make the endpoint 1/n of the way through the file
	fseek(fp, endpoint, SEEK_CUR);	//go to the endpoint

	while ((c = fgetc(fp)) != EOF && c != 32 && c != 10);	//go to the end of the current word												
	endpoint = ftell(fp);	//adjust endpoint to be on the start of the next word

	//save these points for the parents rang to read
	startpointParent = startpoint;
	endpointParent = endpoint;

	//creates the pipes are the child processes
	for (i=0; i < n-1; i++){
		startpoint = endpoint;	//make the next startpoint the last endpoint
		endpoint += fileSize/n;	//move the endpoint by 1/n the filesize
		if(endpoint >= fileSize)	//if addition overshot the filesize
			endpoint = EOF;			//make endpoint EOF
		else{
			fseek(fp, endpoint, SEEK_SET);	//go to the new endpoint
			while ((c=fgetc(fp))!= EOF && c != 32 && c != 10);	//go until end of current word
			endpoint = ftell(fp);			//make the endpoint the start of the next word
			fseek(fp, startpoint, SEEK_SET);	//go to the startpoint
		}

		childNum = i;	//used later to identify pipes

		if (pipe(fd[i]) == -1) {	//if the pipe setup fails
        	perror("pipe setup failed.");	//throw error
        	return 1;	//return 1 to indicate error
   		}
   		
		child_pid = fork ();	//create a child process
		if (child_pid == 0)		//if its the child process
			break;				//break the loop so the children won't make children
	}

	struct word_t *buf;	//buffer for read and write
	buf = (struct word_t *)malloc(sizeof(struct word_t));	//allocate space for a node

	if (child_pid > 0){  //parent process
		//get the parents start and end point
		startpoint = startpointParent;
		endpoint = endpointParent;

		for(i = 0; i < n-1; i++){	//go through n-1 time (the number of pipes)
			read(fd[i][0], buf, sizeof(struct word_t));	//read from the pipe | ***size * list_length***
			printf("%s: %d\n", buf->word, buf->count);	//print out the word its count
			close(fd[i][0]);	//close the read end
		}
	}
	else if (child_pid == 0){	//child process
		buf->word = "hello";	//temporary for testing
		buf->count = childNum;	//temporary to test pipes and childprocesses

		write(fd[childNum][1], buf, sizeof(struct word_t));	//write to the pipe | ***size * list_length***
		close(fd[childNum][1]);	//close the write end
		exit(0);	//exit the child process
	}

	//free pointers
	free(buf);
	free(fd);

	return 0;
}