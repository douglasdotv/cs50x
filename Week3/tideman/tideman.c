#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
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
} pair;

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
void merge_sort(int start, int end);
void merge(int start, int middle, int end);
void lock_pairs(void);
bool detect_cycle(int winner, int loser);
bool traverse(int current, int target, bool visited[]);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }

    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        int winner_index = ranks[i];

        for (int j = i + 1; j < candidate_count; j++)
        {
            int loser_index = ranks[j];
            preferences[winner_index][loser_index]++;
        }
    }

    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            int winner_index;
            int loser_index;

            if (preferences[i][j] > preferences[j][i])
            {
                winner_index = i;
                loser_index = j;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                winner_index = j;
                loser_index = i;
            }
            else
            {
                continue;
            }

            pair new_pair;
            new_pair.winner = winner_index;
            new_pair.loser = loser_index;
            pairs[pair_count++] = new_pair;
        }
    }

    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    merge_sort(0, pair_count - 1);
    return;
}

void merge_sort(int start, int end)
{
    if (start < end)
    {
        int middle = start + (end - start) / 2;
        merge_sort(start, middle);
        merge_sort(middle + 1, end);
        merge(start, middle, end);
    }

    return;
}

void merge(int start, int middle, int end)
{
    int left = middle - start + 1;
    int right = end - middle;

    pair left_pairs[left];
    pair right_pairs[right];

    for (int i = 0; i < left; i++)
    {
        left_pairs[i] = pairs[start + i];
    }

    for (int j = 0; j < right; j++)
    {
        right_pairs[j] = pairs[middle + 1 + j];
    }

    int i = 0;
    int j = 0;
    int k = start;

    while (i < left && j < right)
    {
        int left_strength_of_victory = preferences[left_pairs[i].winner][left_pairs[i].loser];
        int right_strength_of_victory = preferences[right_pairs[j].winner][right_pairs[j].loser];

        if (left_strength_of_victory >= right_strength_of_victory)
        {
            pairs[k++] = left_pairs[i++];
        }
        else
        {
            pairs[k++] = right_pairs[j++];
        }
    }

    while (i < left)
    {
        pairs[k++] = left_pairs[i++];
    }

    while (j < right)
    {
        pairs[k++] = right_pairs[j++];
    }

    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int i = 0; i < pair_count; i++)
    {
        pair current_pair = pairs[i];

        if (!detect_cycle(current_pair.winner, current_pair.loser))
        {
            locked[current_pair.winner][current_pair.loser] = true;
        }
    }

    return;
}

bool detect_cycle(int winner, int loser)
{
    bool visited[candidate_count];

    for (int i = 0; i < candidate_count; i++)
    {
        visited[i] = false;
    }

    return traverse(loser, winner, visited);
}

bool traverse(int current, int target, bool visited[])
{
    if (current == target)
    {
        return true;
    }

    visited[current] = true;

    for (int i = 0; i < candidate_count; i++)
    {
        if (locked[current][i] && !visited[i])
        {
            if (traverse(i, target, visited))
            {
                return true;
            }
        }
    }

    return false;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        bool is_source = true;

        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                is_source = false;
                break;
            }
        }

        if (is_source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
}
