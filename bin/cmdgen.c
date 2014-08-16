#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

int main (int argc, char * argv[])
{
	int f = 0;
	unsigned long d;
	unsigned long a;
	int retval;
	
	
	if (argc > 1) {
		f = open(argv[1], O_RDONLY);
		if (f < 0) {
			fprintf(stderr, "Error while opening file '%s': %s\n", argv[1], strerror(errno));
			return 1;
		}
	}
	
	a = 0x202000;
	while ((retval = read(f, &d, 4)) > 0) {
		printf("W%X,%X#\n", a, d);
		a += 4;
	}
	
	if (retval < 0) {
		fprintf(stderr, "Error while reading file '%s': %s\n", argv[1], strerror(errno));
		return 2;
	}
	
	if (f > 2) {
		close(f);
	}
	
	return 0;
}
