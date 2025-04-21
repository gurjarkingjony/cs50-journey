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
        // Print spaces
        for (int spaces = height - row - 1; spaces > 0; spaces--)
        {
            printf(" ");
        }
        // Print hashes for the first pyramid
        for (int hashes = 0; hashes <= row; hashes++)
        {
            printf("#");
        }
        // Print spaces between the pyramids
        printf("  ");
        // Print hashes for the second pyramid
        for (int hashes = 0; hashes <= row; hashes++)
        {
            printf("#");
        }
        printf("\n");
    }
}
