#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (int row = 0; row < height; row++)
    {
        for (int spaces = height - row - 1; spaces > 0; spaces--)
        {
            printf(" ");
        }
        for (int hashes = 0; hashes <= row; hashes++)
        {
            printf("#");
        }
        printf("  ");
        for (int hashes = 0; hashes <= row; hashes++)
        {
            printf("#");
        }
        printf("\n");
    }
}
