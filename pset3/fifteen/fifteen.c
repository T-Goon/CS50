/**
 * fifteen.c
 *
 * Implements Game of Fifteen (generalized to d x d).
 *
 * Usage: fifteen d
 *
 * whereby the board's dimensions are to be d x d,
 * where d must be in [DIM_MIN,DIM_MAX]
 *
 * Note that usleep is obsolete, but it offers more granularity than
 * sleep and is simpler to use than nanosleep; `man usleep` for more.
 */

#define _XOPEN_SOURCE 500

#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// constants
#define DIM_MIN 3
#define DIM_MAX 9

// board
int board[DIM_MAX][DIM_MAX];

// dimensions
int d;

// prototypes
void clear(void);
void greet(void);
void init(void);
void draw(void);
bool move(int tile);
bool won(void);

int main(int argc, string argv[])
{
    // ensure proper usage
    if (argc != 2)
    {
        printf("Usage: fifteen d\n");
        return 1;
    }

    // ensure valid dimensions
    d = atoi(argv[1]);
    if (d < DIM_MIN || d > DIM_MAX)
    {
        printf("Board must be between %i x %i and %i x %i, inclusive.\n",
            DIM_MIN, DIM_MIN, DIM_MAX, DIM_MAX);
        return 2;
    }

    // open log
    FILE *file = fopen("log.txt", "w");
    if (file == NULL)
    {
        return 3;
    }

    // greet user with instructions
    greet();

    // initialize the board
    init();

    // accept moves until game is won
    while (true)
    {
        // clear the screen
        clear();

        // draw the current state of the board
        draw();

        // log the current state of the board (for testing)
        for (int i = 0; i < d; i++)
        {
            for (int j = 0; j < d; j++)
            {
                fprintf(file, "%i", board[i][j]);
                if (j < d - 1)
                {
                    fprintf(file, "|");
                }
            }
            fprintf(file, "\n");
        }
        fflush(file);

        // check for win
        if (won())
        {
            printf("ftw!\n");
            break;
        }

        // prompt for move
        printf("Tile to move: ");
        int tile = get_int();

        // quit if user inputs 0 (for testing)
        if (tile == 0)
        {
            break;
        }

        // log move (for testing)
        fprintf(file, "%i\n", tile);
        fflush(file);

        // move if possible, else report illegality
        if (!move(tile))
        {
            printf("\nIllegal move.\n");
            usleep(500000);
        }

        // sleep thread for animation's sake
        usleep(500000);
    }

    // close log
    fclose(file);

    // success
    return 0;
}

/**
 * Clears screen using ANSI escape sequences.
 */
void clear(void)
{
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);
}

/**
 * Greets player.
 */
void greet(void)
{
    clear();
    printf("WELCOME TO GAME OF FIFTEEN\n");
    usleep(2000000);
}

/**
 * Initializes the game's board with tiles numbered 1 through d*d - 1
 * (i.e., fills 2D array with values but does not actually print them).
 */
void init(void)
{
    int tile= (d*d)-1;

    //assigns the tile values to the board
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            board[i][j]=tile;
            tile--;
        }
    }

    // swaps 1 and 2 if d is even
    if(d%2==0)
    {
        board[d-1][d-2]=2;
        board[d-1][d-3]=1;
    }
}

/**
 * Prints the board in its current state.
 */
void draw(void)
{
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(board[i][j]!=0)
                printf("%d ",board[i][j]);
            else
                printf("_ ");
        }
        printf("\n\n");
    }

}

/**
 * If tile borders empty space, moves tile and returns true, else
 * returns false.
 */
bool move(int tile)
{
    int tileLocationY;
    int tileLocationX;

    // Finds the location of tile
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(board[i][j]==tile)
            {
                tileLocationY=i;
                tileLocationX=j;
                break;
            }
        }
    }

    //checks if the tileLocationY is at the top or bottom of the board
    if(tileLocationY==d-1)
    {
        //tileLocationY is at the bottom of the board so only check for the empty space above and move the tiles
        if(board[tileLocationY-1][tileLocationX]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY-1][tileLocationX];
            board[tileLocationY-1][tileLocationX]=temp;
            return true;
        }

    }
    else if(tileLocationY==0)
    {
        //tileLocationY is at the top of the board so only check for the empty space below and move the tiles
        if(board[tileLocationY+1][tileLocationX]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY+1][tileLocationX];
            board[tileLocationY+1][tileLocationX]=temp;
            return true;
        }
    }
    else
    {
        // The tileLocationY is in the middle of the board so check the one unit above and below the location for the empty space and move
        if(board[tileLocationY-1][tileLocationX]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY-1][tileLocationX];
            board[tileLocationY-1][tileLocationX]=temp;
            return true;
        }
        else if(board[tileLocationY+1][tileLocationX]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY+1][tileLocationX];
            board[tileLocationY+1][tileLocationX]=temp;
            return true;
        }
    }

    // checks if the tileLocationX is at the right or left edge of the board
    if(tileLocationX==d-1)
    {
        if(board[tileLocationY][tileLocationX-1]==0)
        {
            // the tileLocationX is at the right edge so only the left of the tile is checked for the empty space
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY][tileLocationX-1];
            board[tileLocationY][tileLocationX-1]=temp;
            return true;
        }
    }
    else if(tileLocationX==0)
    {
        if(board[tileLocationY][tileLocationX+1]==0)
        {
            //the tileLocationX is at the left edge of the board so only the right of the tile is checked for the empty space
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY][tileLocationX+1];
            board[tileLocationY][tileLocationX+1]=temp;
            return true;
        }
    }
    else
    {
        // the tile is not at the edge of the board so it checks both the right and left of the tile for the empty space
        if(board[tileLocationY][tileLocationX-1]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY][tileLocationX-1];
            board[tileLocationY][tileLocationX-1]=temp;
            return true;
        }
        else if(board[tileLocationY][tileLocationX+1]==0)
        {
            int temp;
            temp=board[tileLocationY][tileLocationX];
            board[tileLocationY][tileLocationX]=board[tileLocationY][tileLocationX+1];
            board[tileLocationY][tileLocationX+1]=temp;
            return true;
        }
    }

    return false;
}

/**
 * Returns true if game is won (i.e., board is in winning configuration),
 * else false.
 */
bool won(void)
{
    int winCondition[d][d];
    int tile=1;

    // creates the winning array
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(tile<d*d)
            {
                winCondition[i][j]=tile;
                tile++;
            }
            else
                winCondition[i][j]=0;
        }
    }

    //checks if the board is the same as the winning array
    for(int i=0;i<d;i++)
    {
        for(int j=0;j<d;j++)
        {
            if(board[i][j]!=winCondition[i][j])
            {
                return false;
            }
        }
    }
    return true;
}
