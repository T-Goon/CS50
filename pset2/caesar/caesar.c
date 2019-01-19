#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main(int argc,string argv[])
{
    if(argv[1]==NULL)
        return 1;

    string input;
    int a;
    int key=atoi(argv[1]);

    if(argc==1||argc>2)
    {
        printf("Invalid number of command line arguments\n");
        return 1;
    }

    printf("plaintext: ");
    input=get_string();

    printf("ciphertext: ");
    for(int i=0, n=strlen(input);i<n;i++)
    {
        if(islower(input[i]))
        {
            a=input[i]-97;
            printf("%c",((a+key)%26)+97);
        }
        else if(isupper(input[i]))
        {
            a=input[i]-65;
            printf("%c",((a+key)%26)+65);
        }

    }
    printf("\n");

}