#include <time.h>
#include <stdio.h>

int main() {
    int epoch = time(NULL);
	for (int i = 0; i < 1000000; i++) {
		__asm__("mov $0x1, %eax\n"
		 		"cpuid");
	}
    printf("%li", time(NULL) - epoch);
}