#include <unistd.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int readFileIntoBuffer(FILE *fp, unsigned char **buffer) {
	fseek(fp, 0, SEEK_END);
	int fileLen = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	*buffer = malloc(fileLen);
	fread(*buffer, fileLen, 1, fp);
	return fileLen;
}

int main(int argc, char **argv) {

	if (argc < 3) {
		printf("Usage: ./packer payload output\n");
		return 1;
	}

	char *payloadPath = argv[1];
	FILE *payload;
	FILE *stub;

	if ((payload = fopen(payloadPath, "rb")) == NULL) {
		printf("Cannot open file %s\n", payloadPath);
		return 1;
	}
	if ((stub = fopen("../stub/stub.bin", "r")) == NULL) {
		printf("Cannot open stub\n");
		return 1;
	}

	unsigned char *stubBuffer;
	int stubLen = readFileIntoBuffer(stub, &stubBuffer);
	fclose(stub);

    //gen key
    srand(time(NULL));
    unsigned char key[2] = {rand() % 256, rand() % 256};
    //patch key
	int keyOffset = 0;
	for (int i = 0; i < stubLen - 1; i++) {
		if (stubBuffer[i] == 0x39 && stubBuffer[i + 1] == 0x30) {
			keyOffset = i;
			stubBuffer[i] = key[0];
			stubBuffer[i + 1] = key[1];
			break;
		}
	}
	
	unsigned char *payloadBuffer;
	int payloadLen = readFileIntoBuffer(payload, &payloadBuffer);
	fclose(payload);
	
	//encrypt payload
	int keyVal = key[1] * 256 + key[0];

	for (int i = 0; i < payloadLen; i++) {
		payloadBuffer[i] = payloadBuffer[i] ^ keyVal % 256;
	}
	unsigned char *packedPayloadBuffer = malloc(stubLen + payloadLen);

	//append bytes
	memcpy(packedPayloadBuffer, stubBuffer, stubLen);
	memcpy(packedPayloadBuffer + stubLen, payloadBuffer, payloadLen);
	
    //write to file
	FILE *packedPayload = fopen(argv[2], "wb");
	fwrite(packedPayloadBuffer, stubLen + payloadLen, 1, packedPayload);
	fclose(packedPayload);
}