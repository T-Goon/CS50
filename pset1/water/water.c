#include <stdio.h>
#include <cs50.h>

int main (void)
{
    int minutes;

    printf("Minutes: ");
    minutes = get_int();
    printf("Bottles : %d\n",(minutes*12));
}