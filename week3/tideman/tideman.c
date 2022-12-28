#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);
bool cycle_detect(int cycle_start, int end);

int main(int argc, string argv[])
{
    // Check for invalid usage of program from the command line
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Determines the number of candidates.
    candidate_count = argc - 1;

    // checks that the number of candidates enters does not exceed 9
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }

    // populates the candidates array with the names of the candidates as they were input on the command line.
    // ex: [john, jim, aden]
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
        //printf("%s\n", candidates[i]); // helpful debugging code!
    }


    // Clear graph of locked in pairs
    // initializes all values to false
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");    // prompts user for the number of voters!

    // Query each voter for their votes
    for (int i = 0; i < voter_count; i++)
    {
        // initializes and empty ranks list with the same number of elements as there are candidates
        int ranks[candidate_count];
        // ranks[i] is voter's ith preference

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // run the vote function
            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        // records the preferences of each voter according to thier full ranks lists
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // iterate over the candidates array.
    for (int i = 0; i < candidate_count; i++)
    {
        // if the name of a candidate in the candidates array matches the name input by the user
        if (strcmp(candidates[i], name) == 0)
        {
            // update ranks at position "rank"
            ranks[rank] = i;
            return true;
        }
    }
    return false;
    // as the user ranks the candidates, their responses are recorded in an array called ranks
    // where each element is the number corresponding to the number that represents that candidate according to the candidates array

    // ex: candiates = [George, Jim, Anna],
    // rank 1: Jim, rank 2: George, rank 3: Anna
    // ranks = [1, 0, 2]
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate over each element of the ranks array
    for (int i = 0; i < candidate_count; i++)
    {
        // iterate over ranks[i+1] so a preference point is awarded to candidate[i] every time the inner loop is completed (i.e. they get a point for every candidate that ranks below them).
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]] += 1;
        }
    }
}

// Record pairs of candidates where one is preferred over the other
// iterate ofver the preferences array
// if one candidate is ranked higher than the other, update the pairs array with the numbers of the winner and loser, then increment the pair count var
//remember! each element of the pairs array is a pair struct that contains a .winner and a .loser
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // iterate through the pairs starting at the end, cementing the lowest pairs in place in doing so.
    for (int i = pair_count - 1; i > 0; i--)
    {
        // interate over the pairs of pairs starting from the beginning, swapping the losing pair downwards.
        // this causes each iteration of the outer loop to move the candidate with the weakest winning margin to the bottom, followed by the next and so on
        for (int j = 0; j < i; j++)
        {
            // for each pair in the pairs array, if the preference count for a pair is less than the pair just to its right, swap the pairs.
            if (preferences[pairs[j].winner][pairs[j].loser] < preferences[pairs[j + 1].winner][pairs[j + 1].loser])
            {
                pair temp = pairs[j];
                pairs[j] = pairs[j+1];
                pairs[j + 1] = temp;
            }
        }
    }
}

// cycle_detect preserves the original cycle_start var (which represents a candidate) and uses recursion
bool cycle_detect(int cycle_start, int end) // (c,a)
{
    if (cycle_start == end)
    {
        return true;
    }

    //iterates over each candidate
    for (int i = 0; i < candidate_count; i++)
    {
        // if there is an existing edge from the ending of the provided pair to any other candidate (i.e. someone is locked in over another), then there is potential that a cycle exists
        // logically, we would want to follow that path to actually SEE if a does indeed cycle exist
        if (locked[end][i] == true)
        {
            // to do so we check if any that candidate represented by "i" is the same as original start by recursively calling cycle_detect.
            if (cycle_detect(cycle_start, i) == true)
            {
                return true;
            }
            //the function will continue to call itself recursively until a.) any version of the function finds an edge, or b.) the losing pair
        }
    }
    return false;


}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // iterate over the pairs array
    for (int i = 0; i < pair_count; i++)
    {
        // run the cycle_detect function
        // if there is no cycle detected
        if (!cycle_detect(pairs[i].winner, pairs[i].loser))
        {
            // lock in the pair
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    // for each candidate, look through the candidates array to see if anyone is pointing at them
    // if they are not, add one to the arrow counter.
    // if they are the source of the graph then the arrow count will match the number of candidates which demonstrates that no one is pointing at them!
    for (int i = 0; i < candidate_count; i++)
    {
        int arrow_count = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i] == false)
            {
                arrow_count += 1;
                if (arrow_count == candidate_count)
                {
                    printf("%s\n", candidates[i]);
                }
            }
        }
    }
    return;
}