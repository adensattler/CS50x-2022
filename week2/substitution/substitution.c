#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

void encrypt(char c, string cipher);
void outline(string cipher, string ptext);
bool checkvalid(string key);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }

    if (!checkvalid(argv[1]))
    {
        printf("Key must contain 26 characters.\n");
        return 1;
    }

    string plaintext = get_string("plaintext: ");


    printf("ciphertext: ");
    outline(argv[1], plaintext);
    printf("\n");
    return 0;
}

bool checkvalid(string key)
{
    // checks if key is 26 characters long
    int len = strlen(key);
    if (len != 26)
        return false;

    // checks if key is composed only of alphabetical characters
    for (int i = 0; i < 26; i++)
    {
        if (!isalpha(key[i]))
            return false;
    }

    // checks if key has repeated letters
    for (int i = 0; i < 26; i++)
    {
        for (int j = i + 1; j <26; j++)
        {
            if (toupper(key[i]) == toupper(key[j]))
            {
                return false;
            }
        }
    }
    return true;
}


void outline(string cipher, string ptext)
{
    for (int i = 0, n = strlen(ptext); i < n; i++)
    {
        if (isalpha(ptext[i]))
            encrypt(ptext[i], cipher);
        else
            printf("%c", ptext[i]);
    }
}

void encrypt(char c, string cipher)
{
    string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < 26; i++)
    {
        if (isupper(c))
        {
            if (alpha[i] == c)
            {
                printf("%c", toupper(cipher[i]));
            }
        }
        if (islower(c))
        {
            if (alpha[i] == toupper(c))
            {
                printf("%c", tolower(cipher[i]));
            }
        }
    }
}