#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int main (int argc, string argv[])
{
    string input;
    int j=0,b;

    if(argc==1||argc>2)
    {
        printf("Invalid command line argument(s)\n");
        return 1;
    }
    for(int i=0,n=strlen(argv[1]);i<n;i++)
    {
        if(!isalpha(argv[1][i]))
        {
            printf("Invalid command line argument(s)\n");
            return 1;
        }
    }

    printf("plaintext: ");
    input=get_string();
    printf("ciphertext: ");

    for(int i=0,n=strlen(input);i<n;i++ )
    {
        if(input[i]==32)
            printf(" ");
          if(isupper(argv[1][j]))
          {
                b= argv[1][j]-65;
          }else{
               b= argv[1][j]-97;
           }
         if(isupper(input[i])&&isalpha(input[i]))
          {
              int a=(((input[i]-65)+b)%26)+65;
              printf("%c",a);
              j=(j+1)%strlen(argv[1]);
          }else if (islower(input[i])&&isalpha(input[i]))
          {
             int a=(((input[i]-97)+b)%26)+97;
             printf("%c",a);
             j=(j+1)%strlen(argv[1]);
          }else
          {
              printf("%c",input[i]);
          }


    }
    printf("\n");

}