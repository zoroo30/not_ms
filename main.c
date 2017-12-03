#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

//   background   color
//0F black and white
//9F blue and white
//CF red and white
//F9 white and blue

// if you opened an open cell with right number of flags then another cell opened with the right number of flags it opens the 2nd cell surrounding cells too

typedef struct {
    int value;
    int flags;
    int is_open;
    int is_flagged;
    int is_question;
} cell;

void isValid(int cols,int rows){
   while(cols <=0 || rows <=0) //cols & rows validation >>positive numbers only<<<
   {
       printf("Invalid!\n");
       printf("Enter cols & rows:\n");
       scanf("%d %d", &cols, &rows);
   }
}

int board_created = 0, lost = 0, win = 0, first_move = 1, moves = 0, flags = 0, questions = 0, mines = 0;

int main()
{
    srand(time(NULL));

    int rows, cols, x, y, x_mv, y_mv;
    char action;
    printf("Enter cols & rows:\n");
    scanf("%d %d", &cols, &rows);
    isValid(cols,rows);
    cell cells[cols][rows];

    for(y=0; y<rows; y++) {
        for(x=0; x<cols; x++) {
            cells[x][y].value = 0;
            cells[x][y].flags = 0;
            cells[x][y].is_open = 0;
            cells[x][y].is_flagged = 0;
            cells[x][y].is_question = 0;
        }
    }

    display_board(rows, cols, (cell*)cells);

    while(!lost && !win) {

        if(!win){
            printf("Enter Your Move : X Y Action\n");
            scanf("%d %d %c", &x_mv, &y_mv, &action);

            if(action != 'q' && action != 'f' && cells[x_mv-1][y_mv-1].is_open == 0)
                moves++;

            open_cell(x_mv-1, y_mv-1, action, rows, cols, (cell*)cells);
        }
        display_board(rows, cols, (cell*)cells);

        if(first_move && action != 'f') {first_move = 0;}

        if(lost) {
            printf("\nYOU LOST THE GAME !! WOULD YOU GIVE UP ?");
            return 0;
        }

        if(check_win(rows, cols, (cell*)cells)){
            win = 1;
            display_board(rows, cols, (cell*)cells);
            printf("\nDID YOU KNOW? THERE ARE MORE THAN 9999999999 PEPOLE WHO FINISHED BIGGER BOARDS IN LESS TIME THAN YOURS!!\n\nJUST KIDDING ,CONGRATULATIONS!");
        }

    }
    return 0;
}

void create_board(int x, int y, int rows, int cols, cell* cells)
{
    int rnd_x, rnd_y, i, j;
    while(mines != 0)
    {
        //printf("mines : %d \n",mines);
        do
        {
            rnd_x = (rand()%cols);
            rnd_y = (rand()%rows);
        }while((rnd_x == x-1 && rnd_y == y-1) || (rnd_x == x-1 && rnd_y == y) || (rnd_x == x-1 && rnd_y == y+1)
                || (rnd_x == x && rnd_y == y-1) || (rnd_x == x && rnd_y == y) || (rnd_x == x && rnd_y == y+1)
                || (rnd_x == x+1 && rnd_y == y-1) || (rnd_x == x+1 && rnd_y == y) || (rnd_x == x+1 && rnd_y == y+1));

        if((cells + rnd_x*rows + rnd_y)->value != 9) {
            (cells + rnd_x*rows + rnd_y)->value = 9;

            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if((cells + (rnd_x+j)*rows + (rnd_y+i))->value != 9 && rnd_x+j < cols && rnd_x+j >= 0 && rnd_y+i < rows && rnd_y+i >= 0)
                        (cells + (rnd_x+j)*rows + (rnd_y+i))->value += 1;

            mines--;
        }
    }
}

void display_board(int rows, int cols, cell* cells) {
    int y, x;

    clrscr();

    mines = 1+(cols*rows)/10;
    printf("moves : %d;  F : %d;  ? : %d;  mines : %d; \n\n", moves, flags, questions, mines);

    for(y=0; y<rows; y++) {
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("%2d ",y+1);
        for(x=0; x<cols; x++)   {
            if((cells + x*rows + y)->is_open == 1){
                if((cells + x*rows + y)->value == 9){
                    if(!win)
                        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
                    else
                        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 042);
                    printf(" * ");
                }
                else if((cells + x*rows + y)->value != 0) {
                    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 121);
                    printf(" %d ",(cells + x*rows + y)->value);
                }
                else{
                    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 249);
                    printf(" %d ",(cells + x*rows + y)->value);
                }
            } else if((cells + x*rows + y)->is_flagged == 1) {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 237);
                printf(" F ");
            } else if((cells + x*rows + y)->is_question == 1) {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 175);
                printf(" ? ");
            } else {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
                printf(" X ");
            }
        }
        printf("\n");
    }
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
    printf("   ");
    for(x=0; x<cols; x++) {
        printf("%2d ",x+1);
    }
    printf("\n");
}

void open_cell(int x, int y, char action, int rows, int cols, cell* cells) {
    int i, j;

    if(board_created != 1){
        if(action != 'f' && action != 'q')
        {
            create_board(x, y, rows, cols, (cell*)cells);
            board_created = 1;
        }
    }

    if(action == 'q' && (cells + x*rows + y)->is_open == 0 && (cells + x*rows + y)->is_question == 0)
    {
            (cells + x*rows + y)->is_question = 1;
            if((cells + x*rows + y)->is_flagged == 1){
                (cells + x*rows + y)->is_flagged = 0;
                flags--;
            }
            questions++;
    }
    else if(action == 'q' && (cells + x*rows + y)->is_question == 1)
    {
            (cells + x*rows + y)->is_question = 0;
            questions--;
    }

    if(action == 'f' && (cells + x*rows + y)->is_open == 0 && (cells + x*rows + y)->is_flagged == 0)
    {
            (cells + x*rows + y)->is_flagged = 1;
            (cells + x*rows + y)->is_question = 0;
            flags++;

            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if((cells + (x+j)*rows + (y+i))->flags != 9 && x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                        (cells + (x+j)*rows + (y+i))->flags += 1;

    }
    else if ((action == 'f' || action == 'q') && (cells + x*rows + y)->is_flagged == 1)
    {
            (cells + x*rows + y)->is_flagged = 0;
            flags--;

            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if((cells + (x+j)*rows + (y+i))->flags != 9 && x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                        (cells + (x+j)*rows + (y+i))->flags -= 1;

    }

    int old_moves = 0;

    if(!first_move){
        if((cells + x*rows + y)->is_open == 1){
            //if((cells + x*rows + y)->value != 0){
                if((cells + x*rows + y)->flags >= (cells + x*rows + y)->value && action != '-')
                    for(j = -1; j <= 1; j++)
                        for(i = -1; i <= 1; i++)
                            if(x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                                if((cells + (x+j)*rows + y+i)->is_open != 1)
                                {
                                    open_cell(x+j,y+i, '-',rows,cols,cells);
                                    if((cells + (x+j)*rows + y+i)->is_flagged != 1)
                                        old_moves++;
                                }
            //}
        }
    }

    if(old_moves != 0) {moves++;}

    if(action == 'f' || action == 'q' || (cells + x*rows + y)->is_flagged == 1 || (cells + x*rows + y)->is_question == 1 || (cells + x*rows + y)->is_open == 1 )
        return;
    else {
        (cells + x*rows + y)->is_open = 1;
        if((cells + x*rows + y)->value == 0){
            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if(x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                        open_cell(x+j,y+i, 'o',rows,cols,cells);
        }
        else if((cells + x*rows + y)->value == 9) {
            lost = 1;
        }
    }
}

int check_win(int rows, int cols, cell* cells) {
    int x, y, cells_left = 0;

    for(y=0; y<rows; y++) {
        for(x=0; x<cols; x++) {
            if((cells + x*rows + y)->is_open == 0){
                cells_left++;
            }
        }
    }

    if(cells_left == mines)
    {
        for(y=0; y<rows; y++) {
            for(x=0; x<cols; x++) {
                if((cells + x*rows + y)->is_open == 0){
                    (cells + x*rows + y)->is_open = 1;
                }
            }
        }
        return 1;
    }

    return 0;
}

void clrscr()
{
    system("@cls||clear");
}
