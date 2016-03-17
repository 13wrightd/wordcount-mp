// Daniel Wright	diw5233@psu.edu
// Evan Gutman		elg5195@psu.edu


#include <stdio.h>
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

int main(int argc, char *argv[]) {
	if(argc != 4){ //The user has to run the program with 4 arguments
		printf("Invalid number of arguments. Please try running again with proper number of arguments.\narg1(input_file) arg2(output_file) arg3(output_file)\n");
	}
	else{

		clock_t begin, end;				//For recording execution time
		double totTime;					//to store the execution tme

		begin = clock();				//begin timing

	    FILE *fp;						//for the input file
	    fp = fopen(argv[1], "r");		//open the input file
	    char *str;
	    char c;
	    int numChar = 0;				//Number of characters in a string
	    struct word_t *head;			//Linked list
	    struct word_t *curr;			//iterator
	    bool found = false;				//for searching the linked list

	    head = (struct word_t *)malloc(sizeof(struct word_t));	//allocate space for the linked list
	    curr = head;	//start at the begining of the list

	    str = (char *)malloc(sizeof(char));
	    bool badCharacter=0;	//for handling double \n

	    if (fp) {	//if the file opened
	        while ((c=fgetc(fp))!=EOF){	//go to the end of the file
	            if((c >= 65 && c <= 90) || (c >= 97 && c <= 122) || (c >= 48 && c <= 57)){	//if c is alphanumeric
	                str[numChar] = tolower(c);	//lowercase it
	                numChar++;	//add to length of the char
	                str = (char *)realloc(str, (numChar+1)*sizeof(char));	//allocate space for the next character
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
	                	}
	                	
	            		else if(strcmp(str, curr->word) < 0){	//if str comes before the current word then insert it into the list before
	                        struct word_t *newNode;
	                        newNode = (struct word_t *)malloc(sizeof(struct word_t));
	                        newNode->word = curr->word;
	                        newNode->count = curr->count;

	                        curr->word = str;
	                        curr->count = 1;

	                        newNode->next = curr->next;
	                        curr->next = newNode;

	                        found = true;
	                    }

	                    
	                    else if(strcmp(str, curr->word) > 0){	//if str comes after the current word either increment the iterator or add the word to the end of the list
	                        if(curr->next == NULL){
	                                curr->next = (struct word_t *)malloc(sizeof(struct word_t));
	                                curr->next->word = str;
	                                curr->next->count++;
	                                found = true;
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
	            	str = (char *)malloc(sizeof(char));	//reset str
	            	
	        	}
	        }

	        fclose(fp);	//close the file
	    
		    FILE *fp2;	//file for data output
		    fp2 = fopen(argv[2], "w");	//open file in writing mode


		   	while(curr != NULL){	//go through the entire list
		   		if(curr->word[0]!='\0')
	        		fprintf(fp2, "%s, %d\n", curr->word, curr->count);	//print the data to the file
		     	curr = curr->next;	//increment the iterator
		    }
		    fclose(fp2);	//close the file

		    end = clock();	//stop the execution timer 
		    totTime = (double)(end - begin)/CLOCKS_PER_SEC;	//calculate the run time

		    FILE *fp3;	//file for execution time output
		    fp3 = fopen(argv[3], "w");	//open in append
		    
		    fprintf(fp3, "Runtime: %f\n", totTime);	//print time to the file
		    fclose(fp3);	//close the file
	    }
	    else
	    	printf("The file could not be opened.\n");

	    
	}
	return 0;
}