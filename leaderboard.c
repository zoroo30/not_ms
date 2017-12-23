#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "leaderboard.h"
// function for interface
COORD coord = {0, 0};
void gotoxy (int x, int y)
{

    coord.X = x;
    coord.Y = y;

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
// every player in leader board has name,score


//top ten only
//leaderboard leaders[10]={{"ms",50},{"kk",60},{"zz",70}};
leaderboard leaders[10];

//current players in file
int current=0;
//sort leaderboard
void sorting(leaderboard x[],int n)
{
    int c,d;
    leaderboard swap;
    for (c = 0 ; c < ( n - 1 ); c++)
    {
        for (d = 0 ; d < n - c - 1; d++)
        {
            if (x[d].score < x[d+1].score)
            {
                swap  = x[d];
                x[d]  = x[d+1];
                x[d+1] = swap;
            }
        }
    }
}

//at every win ,, write the array to the file
void to_file(leaderboard a[], int items, const char * fileName)
{
    FILE* ptr = fopen(fileName,"wb");
    if(! ptr) exit(1); // TODO: better error handling
    fwrite( a, sizeof(leaderboard), items, ptr);
    fclose(ptr);
}
//will be used in main menu to show leader board
int from_file(leaderboard a[], const char * fileName)
{
    current = 0;
    FILE* ptr = fopen(fileName,"rb");
    if( !ptr )  return 0;
    int n = 0;
    for (n=0; !feof(ptr); n++)
    {
        if ( fread(&a[n],sizeof(leaderboard),1,ptr) != 1) break;
        current++;
    }
    fclose(ptr);
    return 1;
}

void SetColorAndBackground(int ForgC, int BackC)
{
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
    return;
}
//prints leader board
void print_leaderboard()
{
    //from_file(leaders,"scores.dat");
    int i;
    SetColorAndBackground(12,0);
    printf("%15s\xba\xba\xba\xba\xba\xba\xba\xba\xba\xba\xba LEADERBOARD \xba\xba\xba\xba\xba\xba\xba\xba\xba\xba\xba\n","");
    printf("%15s   %-6s         %-10s %s","","RANK","NAME","SCORE");
    sorting(leaders,3);

    for(i=0; i<current; i++)
    {
        SetColorAndBackground(15,0);
        gotoxy(15,2+i);
        printf("+ÄÄ %02d ÄÄ+  %-15.15s  %d",i+1,leaders[i].player,leaders[i].score);
    }
}
void duplicate()
{
    int i,j,k;
    for(i=0; i<current-1; i++)
    {
        for(j=i+1; j<current; j++)
        {
            if(strcmp(leaders[i].player,leaders[j].player)== 0)
            {
                for(k=j; k<current; k++)
                    leaders[k]=leaders[k+1];
                current--;
            }
        }
    }
}

void search_n_replace(leaderboard x)
{
    int i;
    for(i=0; i<current; i++)
    {

        if(strcmp(leaders[i].player,x.player)== 0)
            if(leaders[i].score < x.score )
            {
                leaders[i].score = x.score;
            }
    }
    duplicate();
}

void add_winner(leaderboard x)
{
    strcpy(leaders[current].player,x.player);
    leaders[current].score=x.score;
    (current)+=1;
    search_n_replace(x);
    sorting(leaders,current);
    to_file(leaders,current,"scores.dat");
    return;
}

int display_leader_board()
{
    int file_exist = from_file(leaders,"scores.dat");
    print_leaderboard();
    return file_exist;
}


