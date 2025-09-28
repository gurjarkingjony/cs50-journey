#include <ctype.h>
#include <stdbool.h>

#include "dictionary.h"

typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;


const unsigned int N = 26;

int num = 0;

node *table[N];

bool check(const char *word)
{
    // TODO
    int index = hash(word);
    node *tmp = table[index];
    while (tmp)
    {
        if (!(strcasecmp(tmp->word, word)))
            return true;
        tmp = tmp->next;
    }
    return false;
}

unsigned int hash(const char *word)
{
    return toupper(word[0]) - 'A';
}

void init(const char *dictionary)
{
    int i = 0;
    while (i < N)
    {
        table[i] = NULL;
        i++;
    }
}
// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    init(dictionary);
    FILE *file;
    if (!(file = fopen(dictionary, "r")))
        return false;
    char word[LENGTH + 1];
    while (fscanf(file, "%s", word) != EOF)
    {
        node *lklma = malloc(sizeof(node));
        if (!lklma)
            return false;
        int index = hash(word);
        strcpy(lklma->word, word);
        lklma->next = table[index];
        table[index] = lklma;
        num++;
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    return num;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    int i = 0;
    while (i < N)
    {
        node *tmp = table[i];
        node *cur = table[i];
        while (cur)
        {
            cur = cur->next;
            free(tmp);
            tmp = cur;
        }
        i++;
    }
    return true;
}
