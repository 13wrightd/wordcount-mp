/* Example of use of fork system call */
#include <stdio.h>
//#include <iostream>
//#include <string>
// Required by for routine
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 

//Node structure for linked list
struct word_t {
	char *word;
	int count;
	struct word_t *next;
};

main(int argc, char *argv[]){

	FILE *fp;
	fp = fopen(argv[1], "r");
	pid_t child_pid;
	int i, n=5;
	int sz;
	int startpoint;
	int endpoint;
	int startpointParent;
	int endpointParent;
	char c;
	int fd[5][2];
	int childNum;
	int *temp;

	temp = (int *)malloc(sizeof(int) * 2);


	// if (pipe(fd) == -1) {
 //        perror("pipe");
 //        exit(1);
 //    }
    



	startpoint=ftell(fp);
	//printf("%d\n\n", startpoint);

	fseek(fp, 0, SEEK_END);
	sz = ftell(fp);

	fseek(fp, 0, SEEK_SET);

	endpoint = sz/n;
	fseek(fp, endpoint, SEEK_CUR);

	while ((c=fgetc(fp)) != EOF && c != 32 && c != 10){
		//printf("%c",c);
		;
	}

	endpoint = ftell(fp);
	//printf("\n next character: %c\n",c);
	//c=fgetc(fp);
	//printf("\n next character: %c\n",c);
	//c=fgetc(fp);
	//printf("\n%d\n", endpoint);


	//printf("%d\n", sz);
	startpointParent = startpoint;
	endpointParent = endpoint;

	for (i=0; i < n-1; i++){
		startpoint = endpoint;
		endpoint += sz/n;
		if(endpoint >= sz)
			endpoint = EOF;
		else{
			fseek(fp, endpoint, SEEK_SET);
			while ((c=fgetc(fp))!= EOF && c != 32 && c != 10);
			endpoint = ftell(fp);
			fseek(fp, startpoint, SEEK_SET);
		}

		childNum = i + 1;

		temp = fd[i];
		if (pipe(temp) == -1) {
        	perror("pipe failed");
        	exit(1);
   		}

		// if((pipe(fd) == -1) || (child_pid == fork() == -1)){
		// 	perror("Failed to set up pipeline.");
		// 	return 1;
		// }
   		

		child_pid = fork ();
		if (child_pid == 0)
			break;
	}



	if (child_pid > 0){  //parent
		startpoint = startpointParent;
		endpoint = endpointParent;

		struct word_t *buf;
		buf = (struct word_t *)malloc(sizeof(struct word_t));

		for(i = 0; i < n-1; i++){
			read(temp[0], buf, sizeof(struct word_t));	//size * n (n = # of nodes)
			printf("%s: %d\n", buf->word, buf->count);
			//, buf->next->word, buf->next->count);
		}
		close(temp[0]);
		//wait(NULL);

	}
	else if (child_pid == 0){ //child
		//printf("child\n");
		struct word_t *num;
		num = (struct word_t *)malloc(sizeof(struct word_t));

		num->word = "hello";
		num->count = childNum;
		//num->next = (struct word_t *)malloc(sizeof(struct word_t));
		//num->next->word = "goodbye";
		//num->next->count = 1;

		write(temp[1], num, sizeof(struct word_t)); //size * n (n = # of nodes)
		close(temp[1]);
		exit(0);

	}


}