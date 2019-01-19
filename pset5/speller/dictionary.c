/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "dictionary.h"

//define trie node
    typedef struct node
    {
        bool isWord;
        struct node *children[27];
    }node;

    node *root;
    unsigned int dictionaryWordCount=0;
    bool isLoaded=false;

//recursive function to free memory
void clear(node *n)
{

//if pointer if null return
    if(n==NULL)
    {
        return;
    }

//iterate throught the children array of the node and if it is not null run clear at the address
    for(int i=0;i<27;i++)
    {
        if(n->children[i]!=NULL)
            clear(n->children[i]);

    }
    free(n);
}

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    int nodeIndex;
    int wordIndex=0;
    char c=word[wordIndex];
    node *cur=root;
    //loop while c is not at the end of a word
    while(c != '\0')
    {
        //determine which index to go to
        if(isupper(c))
        {
            nodeIndex=c-64;
        }
            else if(c==39)
            {
                nodeIndex=0;
            }
                else
                {
                    nodeIndex=c-96;
                }
        //if there is nothing there before the word finishes then it is misspelled
        if(cur->children[nodeIndex]==NULL)
        {
            return false;
        }
            else
            {
                cur=cur->children[nodeIndex];
            }
        //get the next letter in the word
        wordIndex++;
        c=word[wordIndex];

    }

    if(cur->isWord==true)
    {
        return true;
    }else
        return false;

}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary in read mode
    FILE *file = fopen(dictionary, "r");

    //malloc memory for root and set the current node to root
    root = calloc(1,sizeof(node));
    node *cur = root;

    //get character of dictionary file
    char c= fgetc(file);
    int index;
    //continues to iterate until the end of the file
    while(c!=EOF)
    {
        //if c is a new line character set the current node to indicate the end of a word and set the current node
        //back to root
        if(c=='\n')
        {
            cur->isWord=true;
            cur = root;
            dictionaryWordCount++;
        }
        //calculate the index based on letter '/''= 0, 'a'=1, 'z'=26
                else
                {
                    if(c==39)
                    {
                        index=0;
                    }
                    else
                        index=c-96;
                    //if there is not a node at the index then create a new one
                     if(cur->children[index]==NULL)
                        {
                            cur->children[index]=calloc(1,sizeof(node));
                            cur=cur->children[index];
                        }
                            //if there is one then move to that node
                            else
                            {
                                cur=cur->children[index];
                            }
                }

        //get the next character in the file
        c= fgetc(file);
    }

//when the end of the file is reached close the file and return true else return false
    if(c==EOF)
    {
        fclose(file);
        isLoaded=true;
        return true;
    }
        else
            return false;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //checks if the dictionary is load and if it is return the number of words(counted in load())
    if(!isLoaded)
        return 0;
    else
        return (dictionaryWordCount);
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    //if the dictionary is loaded free the memory if not return false
    if(isLoaded)
    {
        clear(root);
        return true;
    }

    return false;
}

