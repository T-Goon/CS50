/**
 * Copies a BMP piece by piece, just because.
 */

#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"

int main(int argc, char *argv[])
{
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: ./resize n infile outfile\n");
        return 1;
    }

    // remember filenames
    char *infile = argv[2];
    char *outfile = argv[3];
    int n = atoi(argv[1]);

    if(n< 0 || n> 100)
		{
				printf("The resize factor should be a positive integer <= 100.\n");
				return 2;
}

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        fprintf(stderr, "Could not open %s.\n", infile);
        return 3;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 4;
    }

    // read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);
    int width= bi.biWidth;
    int height = abs(bi.biHeight);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 5;
    }

    // determine padding for scanlines
    int padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biHeight=bi.biHeight*n;
    bi.biWidth=bi.biWidth*n;
    int paddingOUT = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    bi.biSizeImage=((sizeof(RGBTRIPLE)*bi.biWidth)+paddingOUT)*abs(bi.biHeight);
    bf.bfSize=bi.biSizeImage+sizeof(BITMAPFILEHEADER)+sizeof(BITMAPINFOHEADER);

    // write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);



    // iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // iterate over pixels in scanline
        for(int b=0; b<n;b++)
        {
            for (int j = 0; j < width; j++)
            {


                // temporary storage
                RGBTRIPLE triple;

                // read RGB triple from infile
                fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

                // write RGB triple to outfile
                for(int a=0;a<n;a++)
                fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);

            }
            // skip over padding, if any
        fseek(inptr, padding, SEEK_CUR);


        // then add it back (to demonstrate how)
        for (int k = 0; k < paddingOUT; k++)
        {
            fputc(0x00, outptr);
        }

        if(b<n-1)
            fseek(inptr,  -(width * 3 + padding ), SEEK_CUR);

        }

    }

    // close infile
    fclose(inptr);

    // close outfile
    fclose(outptr);

    // success
    return 0;
}
