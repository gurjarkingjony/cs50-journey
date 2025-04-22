// Implements a spell-checker

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"

// Default dictionary
#define DICTIONARY "dictionaries/large"

// Prototype
double calculate(const struct rusage *b, const struct rusage *a);

int main(int argc, char *argv[])
{
    // Benchmarking structures
    struct rusage before, after;

    // Check for correct usage
    if (argc != 2 && argc != 3)
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }

    // Dictionary file
    char *dictionary = (argc == 3) ? argv[1] : DICTIONARY;

    // Text file
    char *text = (argc == 3) ? argv[2] : argv[1];

    // Load dictionary
    getrusage(RUSAGE_SELF, &before);
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // Check for loading errors
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // Calculate time to load
    double time_load = calculate(&before, &after);

    // Open text
    FILE *file = fopen(text, "r");
    if (!file)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1];

    // Spell-checking time
    getrusage(RUSAGE_SELF, &before);

    for (int c = fgetc(file); c != EOF; c = fgetc(file))
    {
        // Accept alphabetic characters and apostrophes inside a word
        if (isalpha(c) || (c == '\'' && index > 0))
        {
            // Append character to word
            if (index < LENGTH)
            {
                word[index] = c;
                index++;
            }
            else
            {
                // Skip the rest of the word
                while ((c = fgetc(file)) != EOF && isalpha(c));
                index = 0;
            }
        }
        else if (index > 0)
        {
            // Terminate word
            word[index] = '\0';
            words++;
            // Check word
            if (!check(word))
                misspellings++;
            index = 0;
        }
    }

    getrusage(RUSAGE_SELF, &after);
    double time_check = calculate(&before, &after);

    // Close text
    fclose(file);

    // Determine dictionary size
    getrusage(RUSAGE_SELF, &before);
    unsigned int total_words = size();
    getrusage(RUSAGE_SELF, &after);
    double time_size = calculate(&before, &after);

    // Unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);
    double time_unload = calculate(&before, &after);

    // Check for unload errors
    if (!unloaded)
    {
        printf("Could not unload dictionary.\n");
        return 1;
    }

    // Report
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %u\n", total_words);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    return 0;
}

// Calculate time in seconds between two rusage values
double calculate(const struct rusage *b, const struct rusage *a)
{
    return (((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
             (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
            ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
             (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec))) /
           1000000.0;
}
