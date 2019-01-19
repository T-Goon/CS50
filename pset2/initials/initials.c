#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    bool space=true;
    string name;
    name= get_string();

    for(int i=0, n=strlen(name);i<n;i++)
    {
        if(name[i]==' ')
        {
            space= true;
        }
        else if (name[i]!=' '&&space ==true)
        {
            printf("%c",toupper(name[i]));
            space =false;
        }

    }
    printf("\n");
}