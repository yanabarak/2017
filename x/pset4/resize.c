/**
 * copy.c
 *
 * Computer Science 50
 * Problem Set 4
 *
 * Copies a BMP piece by piece, just because.
 */
       
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char* argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        printf("Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    int n = atoi(argv[1]);
    char* infile = argv[2];
    char* outfile = argv[3];
    if (n < 1 || n > 100)
    {
        printf("n < 1 and n > 100");
        return 1;  
    }

    // open input file 
    FILE* inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE* outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 || 
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }
    int biWidth = bi.biWidth; 
    int biHeight = bi.biHeight;
    int padding1in =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    //int bfSize = bf.bfSize;
    //int biSizeImage = bi.biSizeImage;
    bi.biWidth *= n; 
    bi.biHeight *= n;
    int padding2out =  (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;
    bf.bfSize = (sizeof(RGBTRIPLE)*bi.biWidth + padding2out)*abs(bi.biHeight) + sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER); 
    bi.biSizeImage = (sizeof(RGBTRIPLE)*bi.biWidth + padding2out)*abs(bi.biHeight); 
    
    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // determine padding for scanlines

    // iterate over infile's scanlines
    for (int i = 0; i < abs(biHeight); i++)
    {
        for(int d = 0; d < n; d++)
        {
        // iterate over pixels in scanline
            for (int j = 0; j < biWidth; j++)
            {
                  // temporary storage
                        RGBTRIPLE triple;

                        // read RGB triple from infile
                         fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                        // write RGB triple to outfile
                    for(int l = 0; l < n; l++)
                    { 
                        fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
                    }
            }

            // then add it back (to demonstrate how)
            for (int k = 0; k < padding2out; k++)
            {
                fputc(0x00, outptr);
            }
            
             if (d < n - 1)
            {
                long int poty = biWidth * sizeof(RGBTRIPLE);
                fseek(inptr, -poty, SEEK_CUR);
            }
            
        }
            
            
        fseek(inptr, padding1in, SEEK_CUR);
            
        
    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // that's all folks
    return 0;
}
