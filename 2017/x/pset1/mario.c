#include <stdio.h>
#include <cs50.h>


	int main(void) {
		int height;
		do {
			printf("Give me an integer number between 0 and 23:");
			height = GetInt();
		}
		while ((height > 23) || (height < 0));
		for (int i = 1; i <= height; i++) {
			for (int y = height - i; y > 0; y--) {
				printf(" ");
			}
			for (int z = i; z > 0; z--) {
				printf("#");
			}
			printf("  ");
			for (int z = i; z > 0; z--) {
				printf("#");
			}
			printf("\n");
		}
	}