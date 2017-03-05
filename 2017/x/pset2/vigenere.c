#include <stdlib.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void funcvigenere(string str, string key);
int main(int argc, string argv[]) 
{
	if (argc != 2) 
	{
		printf("Usage: ./vigenere k\n");
		return 1;
	} 
	else 
	{
		string key = argv[1];
		for (int z = 0, j = strlen(key); z < j; z++) 
		{
			if (!(isalpha(key[z]))) 
			{
				printf("Usage: ./vigenere k\n");
				return 1;
			}
		}
	}
	string key = argv[1];
	printf("plaintext: ");
	string p = GetString();
	printf("ciphertext: ");
	funcvigenere(p, key);
}

void funcvigenere(string str, string key) 
{
	int jt = 0, li = 0;
	char htc = 0;
	for (int i = 0, n = strlen(str); i < n; i++) 
	{
		int o = strlen(key);
		if (isalpha(str[i])) 
		{
			jt = jt % o;
			htc = toupper(key[jt]);
			li = (htc - 65) % 26;
			if (isupper(str[i])) 
			{
				int l = ((str[i] - 65) + li) % 26;
				printf("%c", (char) l + 65);
			}
			if (islower(str[i])) 
			{
				int l = ((str[i] - 97) + li) % 26;
				printf("%c", (char) l + 97);
			}
			jt++;
		} 
		else 
		{
			printf("%c", str[i]);
		}
	}
	printf("\n");
}