// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "dictionary.h"


// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 100000;

// Hash table
node *table[N];

// variable that will be incremented every time a word is added to the hash table
int word_count = 0;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // this block makes an array of characters, copys the original charaters into that array, and then sets each char to lowercase
    char lower_word[LENGTH + 1];
    strcpy(lower_word, word);
    for (int i = 0; word[i] != '\0'; i++)
    {
        lower_word[i] = tolower(lower_word[i]);
    }

    // determine which bucket the words belongs in
    int hash_bucket = hash(lower_word);
    // examine the first node
    node *cursor = table[hash_bucket];
    while (cursor != NULL)
    {
        if (strcasecmp(lower_word, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }
    }


    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO:
    unsigned int hash_sum = 0;
    for (int i=0; word[i] != '\0'; i++)
    {
        hash_sum += (toupper(word[i]) - 'A')*(pow(26, (strlen(word)-(i+1))));
    }
    return hash_sum % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // Checks if the dictionary file paramter can actually be opened and read! (quits if not)
    FILE *dict_file = fopen(dictionary, "r");
    if (dict_file == NULL)
    {
        printf("Could not open file.\n");
        return false;
    }

    // initializes a variable called word to store a string
    char word[LENGTH + 1];
    // scans the dictionary file line by line until the end of the file. stores each line as a string in the var called word
    while (fscanf(dict_file, "%s", word) != EOF)
    {
        // creates an empty node and mallocs space for it on the heap
        node *n = malloc(sizeof(node));
        if (n == NULL)
        {
            return false;
        }

        // fills the new node fields with the word and thext pointer initialized to NULL
        strcpy(n->word, word);
        n->next = NULL;

        // hash the word!
        unsigned int hash_value = hash(word);
        // if there is nothing in that hash bucket, set the bucket equal to the node
        if (table[hash_value] == NULL)
        {
            table[hash_value] = n;
        }
        else
        {
            n->next = table[hash_value];
            table[hash_value] = n;
        }
        word_count++;
    }
    fclose(dict_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i <= N; i++)
    {
        // make a new node that starts at each bucket
        node *n = table[i];

        while(n != NULL)
        {
            node *tmp = n;
            n = n->next;
            free(tmp);
        }
    }

    return true;
}

