/*
    Arguments (foreground process)
    Description: Prints command line arguments
*/

#include <stdio.h>
int
main(int argc, char* argv[])
{
	if (argc >= 2) {
		int i;
		printf("------------------------------------------------\n");
		for (i = 0 ; i < argc; ++i) {
			fprintf(stderr, "Number of arguments %d: %s\n", i, argv[i]);
		}
		printf("------------------------------------------------\n");
	} else {
		fprintf(stderr, "Please enter at least one command line argument\n");
		fprintf(stderr, "Example: `create ./args hello world`\n");
	}
	return 0;
}

