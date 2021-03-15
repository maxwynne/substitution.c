#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();
// Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }
// Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);
// If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }
// Eliminate anyone with minimum number of votes
        eliminate(min);
// Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// voter 0 (1st voter) votes for Bob as his preferred candidate. Bob is at index 1(2nd place) in our candidates array. So vote will add to the array preferences[0][0] = 1; 
// then return true, which makes the main function ask the voter for his second preference. Suppose now the voter enters Alice, she is at index 0 in our candidates array. 
// Our vote function now will add preferences[0][1] = 0; and so on.
// Record preference if vote is valid.
bool vote(int voter, int rank, string name)
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (strcmp(name, candidates[c].name) == 0)
        {
            preferences[voter][rank] = c;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
// tabulate: counts votes for the runoff. loops through each voter in our preferences array; loops through each rank.
// to make sure we are taking preferences correctly, we run an if check for each candidate bool value
// if candidate is still running for election, we have false and our check adds 1 (.votes++;) to that candidate vote count and breaks the nested loop
void tabulate(void)
{
    for (int v = 0; v < voter_count; v++)
    {
        for (int r = 0; r < candidate_count; r++)
        {
            int c = preferences[v][r];
            if (candidates[c].eliminated == false)
            {
                candidates[c].votes++;
                break;
            }
        }
    }
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (candidates[c].votes > (voter_count / 2))
        {
            printf("%s\n", candidates[c].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int voteCount = voter_count;
    for (int c = 0; c < candidate_count; c++)
    {
        if (candidates[c].votes < voteCount && candidates[c].eliminated == false)
        {
            voteCount = candidates[c].votes;
        }
    }
    return voteCount;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int tieCount = 0;
    int onRun = 0;

    for (int c = 0; c < candidate_count; c++)
    {
        if (candidates[c].eliminated == false && candidates[c].votes == min)
        {
            tieCount++;
            onRun++;
        }
        else if (candidates[c].eliminated == false)
        {
            onRun++;
        }
    }
    if (tieCount == onRun)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int c = 0; c < candidate_count; c++)
    {
        if (candidates[c].eliminated == false && candidates[c].votes == min)
        {
            candidates[c].eliminated == true
        }
    }
    return;
}
