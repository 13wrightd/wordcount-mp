#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 

main(int argc, char *argv[]){
	char *str;
	str = (char *)malloc( sizeof(char));
	str[0] = 'a';
	printf("str: %s\n", str);
	free(str);
	printf("str: %s\n", str);
	str = (char *)malloc( sizeof(char));
	str[0] = 'b';
	printf("str: %s\n", str);
	free(str);


	return 0;
}