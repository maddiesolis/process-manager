/*
    Infinite loop (background process)
    Description: Prints user input infinitely in intervals specified by user
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		fprintf(stderr, "Please enter command line arguments in the form:\n");
		fprintf(stderr, "`start ./inf hello 30`\n");
	} else {
		const char* tag = argv[1];
		int interval = atoi(argv[2]);
		while(1) {
			printf("%s\n", tag);
			sleep(interval);
		}
	}
	exit(0);
}

