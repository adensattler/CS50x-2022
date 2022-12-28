#include <cs50.h>
#include <stdio.h>

int main(void)
{
    int height, row, column, spaces;
    do
    {
        height = get_int("Height: ");
    }
    while (height < 1 || height > 8);

    for (row = 0; row < height; row += 1)
    {
        for (spaces = 0; spaces < height - row - 1; spaces += 1)
        {
            printf(" ");
        }
        for (column = 0; column <= row; column += 1)
        {
            printf("#");
        }
        printf("  ");
        for (column = 0; column <= row; column += 1)
        {
            printf("#");
        }
        printf("\n");
    }
}