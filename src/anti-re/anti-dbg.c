#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <string.h>
#include <signal.h>

void handler(int signo)
{
}
int main() {
	printf("A");
}
__attribute__((constructor))
void antidbg() {
	signal(SIGTRAP, handler);
	__asm__("int3");
	
}