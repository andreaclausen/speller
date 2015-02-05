/****************************************************************************
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 6
 *
 * Implements a dictionary's functionality.
 ***************************************************************************/

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "dictionary.h"

#define HASHTABLE_SIZE 65536

/**
 * Definitions of global variables.
 */

// node
typedef struct node
{
char entry[LENGTH + 1];
struct node* next;
}
node;

// hashtable
node* hashtable[HASHTABLE_SIZE];

// number of words in dictionary
int num_words = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Convert word to lowercase to enable checking
    char word_lower[strlen(word)+1];
    for (int i=0, n=strlen(word); i<n; i++)
    {
        word_lower[i] = tolower(word[i]);
    }
    word_lower[strlen(word)] = '\0';
    
    // Go to the corresponding bucket
    node* ptr = hashtable[hash_it(word_lower)];
    while (ptr != NULL)
    {
        // search for the word through the linked list
        if (strcmp(word_lower, ptr->entry) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    
    // if cursor reaches NULL then word isn't found
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Open dictionary file and read from it
    FILE* fp = fopen(dictionary, "r");
    if (fp == NULL)
    {
        return false;
    }
    
    char word[LENGTH + 1];
    
    // Read one word at a time until the end of the dictionary
    while(fscanf(fp, "%s", word) > 0)
    {
        // Create a new node for the word
        node* new_node = malloc(sizeof(node));
        strcpy(new_node->entry, word);
        
        // Hash the word
        int index = hash_it(word);
        
        new_node->next = hashtable[index];
        hashtable[index] = new_node;
        
        // Increase count of words in dictionary
        num_words++;
    }
    
    // Close dictionary file and return to main
    fclose(fp);
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // Return the number of words loaded into the dictionary
    return num_words;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    // Go through each head node of the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    { 
        node* cursor = hashtable[i];
    
        while (cursor != NULL)
        {
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}

/**
 * Hash function from user delipity on reddit.
 */
int hash_it(const char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
    {
        hash = (hash << 2) ^ needs_hashing[i];
    }
    return hash % HASHTABLE_SIZE;
}
