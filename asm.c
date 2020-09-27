#include <stdio.h>


int main() {
	int n, p, r;

	printf("n:");
	scanf("%d", &n);
	printf("p:");
	scanf("%d", &p);

	if (p < 0) {
		puts("p должно быть не меньше 0");
		return 0;
	}
	
	asm (
		"mov $1, %%eax\n\t"
	"loop:\n\t"
		"cmp $0, %%ecx\n\t"
		"jz loop_end\n\t"
		"mul %%ebx\n\t"
		"dec %%ecx\n\t"
		"jmp loop\n\t"
	"loop_end:\n\t"
		: "=a" (r)
		: "b" (n), "c" (p)
	);

	printf("(%d)^%d = %d\n", n, p, r);
	return 0;
}
