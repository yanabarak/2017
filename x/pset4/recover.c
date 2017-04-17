/**
 * recover.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Recovers JPEGs from a forensic image.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef uint8_t BYTE;

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        fprintf(stderr,"Usage: ./recover image\n");
        return 1;
    }

    // перемінні
    char* infile = argv[1];
    BYTE data[512];
    int NameJpg = 000;
    FILE* ouptr = 0;
    char fileName[6];
    
    FILE* inptr = fopen(infile, "r");
    
    if (inptr == 0)
    {
        fprintf(stderr,"Could not open %s.\n", infile);
        return 2;
    }
    int reload = 0;
    while(fread(&data, 512, 1, inptr) != 0){
        if((data[0] == 0xff)  && (data[1] == 0xd8) && (data[2] == 0xff) && ((data[3] & 0xf0) == 0xe0)){
                if(reload == 1){
                    fclose(ouptr);
                }
                sprintf(fileName,"%03i.jpg",NameJpg);
                ouptr = fopen(fileName,"w");
                reload = 1;
                NameJpg++;
                if(reload == 1){
                    fwrite(data, 512, 1, ouptr);
                }
        }
        else if(reload == 1){
            fwrite(data, 512, 1, ouptr);
        }
        
    }
    fclose(inptr);
}
