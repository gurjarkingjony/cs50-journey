#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX 9

typedef struct
{
    string name;
    int votes;
}
candidate;

candidate candidates[MAX];

int candidate_count;

bool vote(string name);
void print_winner(void);

int main(int argc, string argv[])
{
    if (argc < 2)
    {
        printf("Usage: plurality [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
    }

    int voter_count = get_int("Number of voters: ");

    for (int i = 0; i < voter_count; i++)
    {
        string name = get_string("Vote: ");

        if (!vote(name))
        {
            printf("Invalid vote.\n");
        }
    }

    print_winner();
}

bool vote(string name)
{
    //Loop through the Candidate count
    for (int i = 0; i < candidate_count; i++)
    {
        //check if candidate is similar to what is user's vote
        if (strcmp(candidates[i].name, name) == 0)
        {
            //if similar to what user vote for, then increase the specific candidate vote and return true
            candidates[i].votes++;
            return true;
        }
    }
    //else if not similar to candidate vote return false.....and print invalid vote
    return false;
}

// Print the winner (or winners) of the election
void print_winner(void)
{
    //Create and variable and set it to 0
    int maximum_vote = 0;

    //iterate over list of candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //check for candidate votes that are  greater than maximum_vote and set them to maximum_vote
        if (candidates[i].votes > maximum_vote)
        {
            maximum_vote = candidates[i].votes;
        }
    }

    //iterate over list of candidate
    for (int i = 0; i < candidate_count; i++)
    {
        //check for candidate votes that are equal to maximum vote and print them as you go
        if (candidates[i].votes == maximum_vote)
        {
            printf("%s\n", candidates[i].name);
        }
    }

    return;
}
