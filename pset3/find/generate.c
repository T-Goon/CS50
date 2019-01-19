/**
 * generate.c
 *
 * Generates pseudorandom numbers in [0,MAX), one per line.
 *
 * Usage: generate n [s]
 *
 * where n is number of pseudorandom numbers to print
 * and s is an optional seed
 */

#define _XOPEN_SOURCE

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// upper limit on range of integers that can be generated
#define LIMIT 65536

int main(int argc, string argv[])
{
    // Makes sure the user inputs the correct number of comand line arguments
    if (argc != 2 && argc != 3)
    {
        printf("Usage: ./generate n [s]\n");
        return 1;
    }

    // Stores the number of values to generate in n
    int n = atoi(argv[1]);

    // if the third c-line argument exists then generate a random number seeded with that argument
    // if there is no third c-line argument then generate a random number seeded with the return value of time()
    if (argc == 3)
    {
        srand48((long) atoi(argv[2]));
    }
    else
    {
        srand48((long) time(NULL));
    }

    // Generates random numbers 'n' times, multiplies them by limit and then prints those numbers out on seperate lines
    for (int i = 0; i < n; i++)
    {
        printf("%i\n", (int) (drand48() * LIMIT));
    }

    // success
    return 0;
}
