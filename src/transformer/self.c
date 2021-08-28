#include <limits.h>
#include <stdlib.h>
#include <stdio.h>    

int main() {
	char *path = realpath("/proc/self/exe", NULL);
	printf("%s", path);
}