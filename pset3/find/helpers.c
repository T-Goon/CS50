/**
 * helpers.c
 *
 * Helper functions for Problem Set 3.
 */

#include <cs50.h>

#include "helpers.h"

/**
 * Returns true if value is in array of n values, else false.
 */
bool search(int value, int values[], int n)
{
    int start=0,end=n;
    int middle=(start+end)/2;

    do{
        if(value==values[middle])
        {
            return true;
        }
        else if(value>values[middle])
        {
            start=middle+1;
            middle=(start+end)/2;
        }else if(value<values[middle])
        {
            end=middle-1;
            middle=(start+end)/2;
        }
    }while(start<=end);

    return false;
}

/**
 * Sorts array of n values.
 */
void sort(int values[], int n)
{
    int frequency[65536];
    int j=0;
    for(int i=0;i<65536;i++)
    {
        frequency[i]=0;
    }

    for(int i=0;i<n;i++)
    {
        frequency[values[i]]++;
    }

    for(int i=0;i<65536;i++)
    {
        if(frequency[i]!=0)
        {
             while(frequency[i]>0)
            {
                values[j]=i;
                frequency[i]--;
                j++;
            }
        }
    }
    return;
}
