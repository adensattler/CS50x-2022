#include <stdio.h>
#include <cs50.h>

int main(void)
{
    int h;
do
{
    h = get_int("Height: ");
}
    while (h < 0 || h > 8);

    for (int r = 0; r < h; r++)
    {
        for (int j = 0; j <= h; j++)
        {
        if ( r + j < (h))
            printf(" ");
        else
            printf("#");
        }
    printf("  ");

        for (int c = 0; c <= r; c++)
        {
            printf("#");
        }

    printf("\n");
    }
}