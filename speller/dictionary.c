#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "dictionary.h"

#define N 10000

// Node structure
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Hash table
node *table[N];

// Word count
unsigned int word_count = 0;

// Hash function
unsigned int hash(const char *word)
{
    unsigned long hash = 5381;
    int c;

    while ((c = *word++))
        hash = ((hash << 5) + hash) + tolower(c);

    return hash % N;
}

// Load dictionary
bool load(const char *dictionary)
{
    FILE *file = fopen(dictionary, "r");
    if (!file)
        return false;

    char word[LENGTH + 1];

    while (fscanf(file, "%45s", word) != EOF)
    {
        node *n = malloc(sizeof(node));
        if (!n)
            return false;

        strcpy(n->word, word);
        int h = hash(word);
        n->next = table[h];
        table[h] = n;
        word_count++;
    }

    fclose(file);
    return true;
}

// Check if word is in dictionary
bool check(const char *word)
{
    char temp[LENGTH + 1];
    int len = strlen(word);

    for (int i = 0; i < len; i++)
        temp[i] = tolower(word[i]);

    temp[len] = '\0';

    int h = hash(temp);
    for (node *n = table[h]; n != NULL; n = n->next)
    {
        if (strcasecmp(n->word, temp) == 0)
            return true;
    }

    return false;
}

// Number of words
unsigned int size(void)
{
    return word_count;
}

// Free memory
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];
        while (cursor)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
