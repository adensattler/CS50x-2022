#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    string text_string = get_string("Text: ");
    int letter_count = 0;
    int word_count = 0;
    int sen_count = 0;

    for (int i = 0, n = strlen(text_string); i<=n; i++)
    {

        if (isalpha(text_string[i]) != 0)
        {
            letter_count++;
        }

        if (text_string[i] == ' ' || text_string[i] == '\0')
        {
            word_count++;
        }

        if (text_string[i] == '!' || text_string[i] == '.' || text_string[i] == '?' )
        {
            sen_count++;
        }
    }
    // printf("%i\n%i\n%i\n", letter_count, word_count, sen_count);

    float L = (100 / (float) word_count) * (float) letter_count;
    float S = (100/(float) word_count) * (float) sen_count;

    int index = round(0.0588 * L - 0.296 * S - 15.8);

    if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", index);
    }

}