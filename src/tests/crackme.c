#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	if (argc < 2) {
		printf("Incorrect ): \n");
		return 0;
	}

	char *pwd = malloc(8);
	memset(pwd, 0x63, 1);
	memset(pwd + 1, 0x6f, 1);
	memset(pwd + 2, 0x6f, 1);
	memset(pwd + 3, 0x6c, 1);
	memset(pwd + 4, 0x5f, 1);
	memset(pwd + 5, 0x70, 1);
	memset(pwd + 6, 0x77, 1);
	memset(pwd + 7, 0x64, 1);

	if (strcmp(pwd, argv[1]) == 0) {
		printf("Correct!\n");
		return 0;
	}
	printf("Incorrect ): \n");
}