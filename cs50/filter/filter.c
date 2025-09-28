#include "helpers.h"
#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        printf("Usage: ./filter [grayscale|reflect|blur|edges] infile outfile\n");
        return 1;
    }

    char *filter = argv[1];
    char *infile = argv[2];
    char *outfile = argv[3];

    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 2;
    }

    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n", outfile);
        return 3;
    }

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 ||
        bi.biSize != 40 || bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 4;
    }

    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    RGBTRIPLE image[height][width];

    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    for (int i = 0; i < height; i++)
    {
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);
        fseek(inptr, padding, SEEK_CUR);
    }

    if (strcmp(filter, "grayscale") == 0)
    {
        grayscale(height, width, image);
    }
    else if (strcmp(filter, "reflect") == 0)
    {
        reflect(height, width, image);
    }
    else if (strcmp(filter, "blur") == 0)
    {
        blur(height, width, image);
    }
    else if (strcmp(filter, "edges") == 0)
    {
        edges(height, width, image);
    }
    else
    {
        printf("Invalid filter.\n");
        fclose(outptr);
        fclose(inptr);
        return 5;
    }

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    for (int i = 0; i < height; i++)
    {
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    fclose(inptr);
    fclose(outptr);

    return 0;
}
