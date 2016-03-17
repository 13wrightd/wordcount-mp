/* Example of use of fork system call */
#include <stdio.h>
//#include <iostream>
//#include <string>
// Required by for routine
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 

main(int argc, char *argv[])
{
	FILE *fp;
	fp = fopen(argv[1], "r");
	pid_t child_pid;
	int i, n=5;
	int position;
	int sz;
	int startpoint;
	int endpoint;
	int startpointParent;
	int endpointParent;
	char c;

	startpoint=ftell(fp);
	printf("%d\n\n", startpoint);

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
			while ((c=fgetc(fp))!=EOF && c != 32 && c != 10);
			endpoint = ftell(fp);
			fseek(fp, startpoint, SEEK_SET);
		}

		child_pid=fork ();
		if (child_pid == 0)
			break;
	}
	if (child_pid > 0)
		printf("parent with range: (%d,%d)\n", startpointParent, endpointParent);
	else if (child_pid == 0)
		printf("child with range: (%d,%d)\n", startpoint, endpoint);
}