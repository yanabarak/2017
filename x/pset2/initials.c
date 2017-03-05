#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

	int main() {
		string name = get_string();
		int l = strlen(name);
		if(name[0] != ' ')
		{
			printf("%c", toupper(name[0]));
		}
		for (int z = 0; z < l - 1; z++) 
		{
			if((name[z] == ' ') && (name[z+1] != ' '))
			{
				printf("%c", toupper(name[z+1]));
			}
		}
		printf("\n");
	}