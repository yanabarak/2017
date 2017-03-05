#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void funccaesar(string str, int k);
int main(int argc, string argv[]) 
{
	if (argc != 2) 
	{
		printf("Usage: ./caesar k\n");
		return 1;
	}
	int k = atoi(argv[1]);
	printf("plaintext: ");
	string p = GetString();
	printf("ciphertext: ");
	funccaesar(p, k);
}

void funccaesar(string str, int k) 
{
	for (int i = 0, n = strlen(str); i < n; i++) 
	{
		if (isalpha(str[i])) 
		{
			if (isupper(str[i])) 
			{
				int l = ((str[i] - 65) + k) % 26;
				printf("%c", (char) l + 65);
			}
			if (islower(str[i])) 
			{
				int l = ((str[i] - 97) + k) % 26;
				printf("%c", (char) l + 97);
			}
		} 
		else 
		{
			printf("%c", str[i]);
		}
	}
	printf("\n");
}