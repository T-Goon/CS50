#include <stdio.h>


int main (int argc, char* argv[])
{
    int count=0;
    char fileName[100];
    FILE *img=NULL;
    int w=0;

    if(argc!=2)
    {
        fprintf(stderr,"Usage: /recover (file name)");
        return 1;
    }

    FILE *card = fopen(argv[1],"r");
    if(card==NULL)
    {
        fprintf(stderr,"File could not be opened.");
        return 2;
    }

    fseek(card, 0, SEEK_END);
	int size = ftell(card);
    int it= size/512;

    fseek(card, 0, SEEK_SET);

    for(int i=0;i<it;i++)
    {
        unsigned char buffer[512];
        w= fread(buffer,1,512,card);

        if(w!=512)
        {
            fclose(card);
            return 0;
        }

        if(buffer[0]==0xff&&buffer[1]==0xd8&&buffer[2]==0xff&&(buffer[3] & 0xf0)== 0xe0)
        {

            if(img!=NULL)
            {
                fclose(img);
            }
            sprintf(fileName,"%03i.jpg",count);

            img = fopen(fileName,"w");
             count++;

        }

        if(img!=NULL)
            fwrite(buffer,512,1,img);

    }

}
