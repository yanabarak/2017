#include <stdio.h>
#include <cs50.h>

	int main(void) {
		int n;
		do {
			printf("Minutes: ");
			n = GetInt();
		}
		while (n < 0);
		printf("Bottles: %i\n", n * 12);
	}