#include<stdio.h>
#include<strings.h>
#include<conio.h>
#include <math.h>
#include<time.h>
#include <windows.h>

//   background   color
//0F black and white
//9F blue and white
//CF red and white
//F9 white and blue

// if you opened an open cell with right number of flags then another cell opened with the right number of flags it opens the 2nd cell surrounding cells too
time_t start_t;
time_t end_t;
int diff_t;

void delay(clock_t a)
{
    clock_t start;
    start  = clock();
    while(clock()-start<a)
    {

    }
}

typedef struct {
    int value;
    int flags;
    int is_open;
    int is_flagged;
    int is_question;
} cell;

int board_created = 0, lost = 0, win = 0, first_move = 1, moves = 0, flags = 0, questions = 0, mines = 0;


int main()
{
    int i;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 11);

printf(
"\n  __   __   _______  _     _  ______   _____   _       _  ______  ______  _____    ______  _____"
"\n (__)_(__) (_______)(_)   (_)(______) (_____) (_)  _  (_)(______)(______)(_____)  (______)(_____)"
"\n(_) (_) (_)   (_)   (__)_ (_)(_)__   (_)___   (_) (_) (_)(_)__   (_)__   (_)__(_) (_)__   (_)__(_)"
"\n(_) (_) (_)   (_)   (_)(_)(_)(____)    (___)_ (_) (_) (_)(____)  (____)  (_____)  (____)  (_____)"
"\n(_)     (_) __(_)__ (_)  (__)(_)____   ____(_)(_)_(_)_(_)(_)____ (_)____ (_)      (_)____ ( ) ( )"
"\n(_)     (_)(_______)(_)   (_)(______) (_____)  (__) (__) (______)(______)(_)      (______)(_)  (_)\n");

    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 7);
    char str[30]="Welcome To Minesweeper !";
    char str1[30]="Are You Ready To Play ?";
    printf(" ");
    for(i=0;str[i]!='\0';i++)
    {
        printf("%c",str[i]);
        delay(60);
    }
    printf("\n");
    printf(" ");
    for(i=0;str1[i]!='\0';i++)
    {
        printf("%c",str1[i]);
        delay(60);
    }
    delay(1500);
    system("cls");
    play_game();

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

void PlayG(int cols,int rows)
{
    int x, y, x_mv, y_mv;
    char action;
    char name[10],tempstr[10];
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
    time(&start_t);
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
            printf("\nYOU LOST THE GAME !");
            printf("\nPress ' Enter ' to get back\n");
            fflush(stdin);
            gets(tempstr);
            continue;
        }

        if(check_win(rows, cols, (cell*)cells)){
            win = 1;
            time(&end_t);
            display_board(rows, cols, (cell*)cells);
            printf("Congratulations!You Won\n");
            diff_t=difftime(end_t, start_t);
            int score=(pow(rows,4)*pow(cols,4))/((moves)*diff_t);
            printf("Your Score is : %d\n",score);
            printf("Enter your name:\n");
            scanf("%s",name);
        }

    }
}

void play_game()
{
    char ch;
    char tempstr[10];
    while(1)
    {
        system("cls");
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x12);
        printf("===MINESWEEPER===\n");
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("-------------------\n");
        printf("(1)Start a new game ");
        printf("\n");
        printf("(2)Show Leaderboard"); //display scores
        printf("\n");
        printf("(3)Load Game\n");//open saved file
        ch=getch();
        if(ch==49)
            {
                system("cls");
              char ch1;
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x12);
              printf("====MINESWEEPER====\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x08);
              printf("Choose difficulty:\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x03);
              printf("(1)Easy(4x4)\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x05);
              printf("(2)Medium(6x6)\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x04);
              printf("(3)Hard(8x8)\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x09);
              printf("(4)Custom(mxn)\n");
              SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
              printf("Press ' Enter ' to get back\n");
              ch1=getch();
              if(ch1==49)
              {
                  PlayG(4,4);
              }
              if(ch1==50)
                {
                    PlayG(6,6);
                }
              if(ch1==51)
                  {
                      PlayG(8,8);
                  }
               if(ch1==52)
              {
                  int rows,cols;
                  system("cls");
                srand(time(NULL));
    printf("Enter rows & columns:\n");
    scanf("%d %d", &rows, &cols);
    PlayG(rows,cols);
              }
            }
            if(ch==50)
        {
            system("cls");
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x12);
            printf("===MINESWEEPER===\n");
            SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
            printf("---------------------------------\n");
            printf("  PLAYER\t\tSCORE\n");
            printf("Press ' Enter ' to get back\n");
            fflush(stdin);
            gets(tempstr);
            continue;
        }
        if(ch==51)
    {
        system("cls");
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x12);
        printf("===MINESWEEPER===\n");
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("Ok!\n");
        printf("Press ' Enter ' to get back\n");
        fflush(stdin);
        gets(tempstr);
        continue;
    }
}
}

void clrscr()
{
    system("@cls||clear");
}

