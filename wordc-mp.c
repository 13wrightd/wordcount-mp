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
	if(argc != 5){ //The user has to run the program with 5 arguments
		printf("Invalid number of arguments. Please try running again with proper number of arguments.\narg1(input_file) arg2(output_file) arg3(output_file) arg4(n)\n");
	}
	else{
		
	}

	return 0;
}