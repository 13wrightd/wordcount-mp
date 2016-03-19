// Daniel Wright	diw5233@psu.edu
// Evan Gutman		elg5195@psu.edu


#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
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
	if(argc != 5){	//check for # of arguments
		printf("Invalid number of arguments. Please try running again with proper number of arguments.\narg1(input_file) arg2(output_file) arg3(output_file) arg4(# of processes)\n");
		return 1;
	}
	else{
		clock_t begin, end;			//for recording time
		double totTime;				//store runtime
		begin = clock();			//start clock

		FILE *fp;					//pointer to file
		pid_t child_pid;			//determine wether child or parent
		int i, j, n = atoi(argv[4]);	//i is a counter, n is the number of processes
		pid_t *children = (pid_t *)malloc((n-1) * sizeof(pid_t));	//stores all children pids
		int fileSize;				//stores size of the file
		int startpoint;				//start point for reading
		int endpoint;				//end point for reading
		int endpointParent;			//stores the parents end point
		char c;						//for reading the file
		char *str = (char *)malloc( sizeof(char));					//to store each string
		int numChar = 0;			//the number of charcters in the string
		struct word_t *head;		//Linked list
	    struct word_t *curr;		//iterator
	    bool badCharacter = 0;		//for handling double \n
	    bool found = false;			//for searching the linked list
	    int *numNodes = (int *)malloc( sizeof(int));
		int **fd = (int **)malloc((n-1) * sizeof(int *));	//dynamic 2d array using points
															//for file descriptors
		int childNum;				//used to access each pipe


		head = (struct word_t *)malloc( sizeof(struct word_t));	//allocate space for the linked list
	    curr = head;	//start at the begining of the list

		for(i = 0; i < n-1; i++){	//make each row have 2 columns for file descriptors
			fd[i] = (int *)malloc(2 * sizeof(int));
		}

		fp = fopen(argv[1], "r");	//opens the file
		if(fp == NULL){				//check if file opened correctly
			perror("failed to open file.");
			return 1;
		}
		else{
			startpoint = ftell(fp);		//gets the startpoint (beginning of file)

			fseek(fp, 0, SEEK_END);		//goes through to the end of the file
			fileSize = ftell(fp);		//gets the last position in file (the length/size)

			fseek(fp, 0, SEEK_SET);		//go back to the beginning of the file

			endpoint = fileSize/n;		//make the endpoint 1/n of the way through the file
			fseek(fp, endpoint, SEEK_CUR);	//go to the endpoint

			while ((c = fgetc(fp)) != EOF && c != 32 && c != 10);	//go to the end of the current word												
			endpoint = ftell(fp);	//adjust endpoint to be on the start of the next word

			//save end point for the parents range to read
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
		        	perror("pipe setup failed.\n");	//throw error
		        	return 1;	//return 1 to indicate error
		   		}
		   		
				child_pid = fork ();	//create a child process
				if (child_pid == -1){
					perror("fork failed.\n");
					return 1;
				}
				else if (child_pid == 0){	//if its the child process
					fclose(fp);			//close the file
					break;				//break the loop so the children won't make children
				}
				else if (child_pid > 0)
					children[i] = child_pid;
			}

			if (child_pid > 0){  //parent process
				//get the parent's end point
				endpoint = endpointParent;
				fseek(fp, 0, SEEK_SET);	
			}
			else if(child_pid == 0){	//child process
				fp = fopen(argv[1], "r");
				if(fp == NULL){
					perror("failed to open file.");
					exit(1);
				}
			}

			while ((c = fgetc(fp))!=EOF && ftell(fp) != endpoint){	//go to the endpoint or break if end of file
	            if((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)){	//if c is alphanumeric
	                str[numChar] = tolower(c);	//lowercase it
	                numChar++;	//add to length of the char
	                str = (char *)realloc(str, (numChar + 1) * sizeof(char));	//allocate space for the next character
	                badCharacter=false;										//not a bad character
	            }

	            else if((c == 32 || c == 10) && !badCharacter){	//if c is a space of \n

	                badCharacter=true;	//handle for possible double
	                str[numChar]='\0';	//add null character to the end of the string
	                numChar = 0;		//reset the string length
	                
	                while(!found){	//go until you find the word in the list

	                	if(curr->word == NULL){	//if the current word is null add str to the node and incerment its counter 
	                		curr->word = str;
	                		curr->count++;
	                		found = true;
	                		*numNodes++;
	                	}
	                	
	            		else if(strcmp(str, curr->word) < 0){	//if str comes before the current word then insert it into the list before
	                        struct word_t *newNode;
	                        newNode = (struct word_t *)malloc( sizeof(struct word_t));
	                        newNode->word = curr->word;
	                        newNode->count = curr->count;

	                        curr->word = str;
	                        curr->count = 1;

	                        newNode->next = curr->next;
	                        curr->next = newNode;

	                        found = true;
	                        *numNodes++;
	                        free(newNode); //check if error occurs
	                    }

	                    
	                    else if(strcmp(str, curr->word) > 0){	//if str comes after the current word either increment the iterator or add the word to the end of the list
	                        if(curr->next == NULL){
	                                curr->next = (struct word_t *)malloc( sizeof(struct word_t));
	                                curr->next->word = str;
	                                curr->next->count++;
	                                found = true;
	                                *numNodes++;
	                        }
	                        else{
	                                curr = curr->next;
	                        }
	                    }
	                	
	                    else{	//else str == curr->word so increment the count
	                        curr->count++;
	                        found = true;
	                    }
	               	}
	               	found = false;	//reset found
	               	curr = head;	//reset the iterator
	               	//free(str);
	            	str = (char *)malloc( sizeof(char));	//reset str
	            	
	        	}
	        }
	        free(str);
	        fclose(fp);	//close the file

	        if(child_pid == 0){	//child process
	        	write(fd[childNum][1], numNodes, sizeof(int));	//write the size of childs list
	        	write(fd[childNum][1], head, sizeof(struct word_t) * (*numNodes));		//write to the pipe
	        	close(fd[childNum][1]);		//close the write end of the pipe
	        	//free(numNodes);
	        	exit(0);
	        }
	        else if(child_pid > 0){		//parent process
	        	int *childNodes;
	        	struct word_t *temp;

	        	for(i = 0; i < n-1; i++){
	        		waitpid(children[i], NULL, 0);

	        		childNodes = (int *)malloc( sizeof(int));
	        		read(fd[i][0], childNodes, sizeof(int));

	        		temp = (struct word_t *)malloc( sizeof(struct word_t));
	        		read(fd[i][0], temp, ((*childNodes) * sizeof(struct word_t)));

	        		close(fd[i][0]);
	        		free(childNodes);

	        		struct word_t *currChild = temp;
	        		while(currChild != NULL){
	        			curr = head;
	        			found = false;
	        			str = currChild->word;

		        		while(!found){	//go until you find the word in the list

		                	if(curr->word == NULL){	//if the current word is null add str to the node and incerment its counter 
		                		curr->word = str;
		                		curr->count = currChild->count;
		                		found = true;
		                	}
		                	
		            		else if(strcmp(str, curr->word) < 0){	//if str comes before the current word then insert it into the list before
		                        struct word_t *newNode;
		                        newNode = (struct word_t *)malloc( sizeof(struct word_t));
		                        newNode->word = curr->word;
		                        newNode->count = curr->count;

		                        curr->word = str;
		                        curr->count = currChild->count;

		                        newNode->next = curr->next;
		                        curr->next = newNode;

		                        found = true;
		                        free(newNode);
		                    }

		                    
		                    else if(strcmp(str, curr->word) > 0){//if str comes after the current word increment the iterator or add the word to the end of the list
		                        if(curr->next == NULL){
		                                curr->next = (struct word_t *)malloc( sizeof(struct word_t));
		                                curr->next->word = str;
		                                curr->next->count = currChild->count;
		                                found = true;
		                        }
		                        else{
		                                curr = curr->next;
		                        }
		                    }
		                	
		                    else{	//else str == curr->word so increment the count
		                        curr->count += currChild->count;
		                        found = true;
		                    }
		               	}
		               	currChild = currChild->next;
	                }
	                //free(temp);	//possible error
	        	}
	        }
	        free(children);
	        free(fd);

	        FILE *fp2;	//file for data output
		    fp2 = fopen(argv[2], "w");	//open file in writing mode

		    curr = head;
		   	while(curr != NULL){	//go through the entire list
		   		if(curr->word[0]!='\0')
	        		fprintf(fp2, "%s, %d\n", curr->word, curr->count);	//print the data to the file
		     	curr = curr->next;	//increment the iterator
		    }
		    fclose(fp2);	//close the file
		    free(head);

		    end = clock();	//stop the execution timer 
		    totTime = (double)(end - begin)/CLOCKS_PER_SEC;	//calculate the run time

		    FILE *fp3;	//file for execution time output
		    fp3 = fopen(argv[3], "w");	//open in append
		    
		    fprintf(fp3, "Runtime: %f\n", totTime);	//print time to the file
		    fclose(fp3);	//close the file
		}
	}
	return 0;
}