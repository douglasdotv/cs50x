// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 150000;

// Hash table
node *table[N];

// Flag to check if the dictionary has been loaded
bool is_dictionary_loaded = false;

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word);
    node *cursor = table[index];

    while (cursor != NULL)
    {
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }

        cursor = cursor->next;
    }

    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    unsigned int total = 0;

    for (int i = 0, len = strlen(word); i < len; i++)
    {
        int n = toupper(word[i]) - 'A';
        total += n * i * (n + i);
    }

    return total % N;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    FILE *dictionary_file = fopen(dictionary, "r");

    if (dictionary_file == NULL)
    {
        return false;
    }

    char word[LENGTH + 1];

    while (fscanf(dictionary_file, "%45s", word) != EOF)
    {
        node *new_node = malloc(sizeof(node));

        if (new_node == NULL)
        {
            fclose(dictionary_file);
            return false;
        }

        strcpy(new_node->word, word);

        int index = hash(word);
        node **head_node = &table[index];
        new_node->next = *head_node;

        *head_node = new_node;
    }

    is_dictionary_loaded = true;

    fclose(dictionary_file);

    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (!is_dictionary_loaded)
    {
        return 0;
    }

    unsigned int word_count = 0;

    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            word_count++;
            cursor = cursor->next;
        }
    }

    return word_count;
}

// Unloads dictionary from memory, returning true if successful
bool unload(void)
{
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        while (cursor != NULL)
        {
            node *next = cursor->next;
            free(cursor);
            cursor = next;
        }
    }

    return true;
}
