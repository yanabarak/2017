#include <stdio.h>
#include <cs50.h>
#include <math.h>

	int main(void) {
		float n;
		int counter = 0;
		do {
			printf("O hai! How much change is owed?\n");
			n = get_float();
		}
		while (n < 0);
		int ns = roundf(n * 100);
		while (ns >= 25) {
			ns = ns - 25;
			counter++;
		}
		while (ns >= 10) {
			ns = ns - 10;
			counter++;
		}
		while (ns >= 5) {
			ns = ns - 5;
			counter++;
		}
		while (ns >= 1) {
			ns = ns - 1;
			counter++;
		}
		printf("%i\n", counter);
	}