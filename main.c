#include<stdio.h>
#include<strings.h>
#include<conio.h>
#include <math.h>
#include<time.h>
#include <windows.h>
#include "leaderboard.h"

//   background   color
//0F black and white
//9F blue and white
//CF red and white
//F9 white and blue

// if you opened an open cell with right number of flags then another cell opened with the right number of flags it opens the 2nd cell surrounding cells too
time_t start_t;
time_t end_t;


void delay(clock_t a)
{
    clock_t start;
    start  = clock();
    while(clock()-start<a)
    {

    }
}

typedef struct
{
    int value;
    int flags;
    int is_open;
    int is_flagged;
    int is_question;
} cell;

int board_created = 0, lost = 0, win = 0, first_move = 1, moves = 0, current_left_cells = 0, flags = 0, questions = 0, mines = 0, timer_start = 0, diff_t = 0, prev_diff_t = 0;


int main()
{
    from_file(leaders,"scores.dat");
    int i;
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 11);

     printf("888b     d888 d8b                    .d8888b."
            "\n8888b   d8888 Y8P                   d88P  Y88b"
            "\n88888b.d88888                       Y88b."
            "\n888Y88888P888 888 88888b.   .d88b.   \"Y888b.   888  888  888  .d88b.   .d88b.  88888b.   .d88b.  888d888"
            "\n888 Y888P 888 888 888 \"88b d8P  Y8b     \"Y88b. 888  888  888 d8P  Y8b d8P  Y8b 888 \"88b d8P  Y8b 888P\""
            "\n888  Y8P  888 888 888  888 88888888       \"888 888  888  888 88888888 88888888 888  888 88888888 888"
            "\n888   \"   888 888 888  888 Y8b.     Y88b  d88P Y88b 888 d88P Y8b.     Y8b.     888 d88P Y8b.     888"
            "\n888       888 888 888  888  \"Y8888   \"Y8888P\"   \"Y8888888P\"   \"Y8888   \"Y8888  88888P\"   \"Y8888  888"
            "\n                                                                               888"
            "\n                                                                               888"
            "\n                                                                               888\n   ");

    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 7);

    //char title[600] = "\n  __   __   _______  _     _  ______   _____   _       _  ______  ______  _____    ______  _____\n (__)_(__) (_______)(_)   (_)(______) (_____) (_)  _  (_)(______)(______)(_____)  (______)(_____)\n(_) (_) (_)   (_)   (__)_ (_)(_)__   (_)___   (_) (_) (_)(_)__   (_)__   (_)__(_) (_)__   (_)__(_)\n(_) (_) (_)   (_)   (_)(_)(_)(____)    (___)_ (_) (_) (_)(____)  (____)  (_____)  (____)  (_____)\n(_)     (_) __(_)__ (_)  (__)(_)____   ____(_)(_)_(_)_(_)(_)____ (_)____ (_)      (_)____ ( ) ( )\n(_)     (_)(_______)(_)   (_)(______) (_____)  (__) (__) (______)(______)(_)      (______)(_)  (_)\n";
    char str[30]="Welcome To Minesweeper !";
    char str1[30]="Are You Ready To Play ?";
    printf(" ");
    for(i=0; str[i]!='\0'; i++)
    {
        printf("%c",str[i]);
        delay(60);
    }
    printf("\n");
    printf(" ");
    for(i=0; str1[i]!='\0'; i++)
    {
        printf("%c",str1[i]);
        delay(60);
    }
    char tempstr[10];
    printf("\nPress ' Enter ' to continue\n");
    fflush(stdin);
    gets(tempstr);

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
        }
        while((rnd_x == x-1 && rnd_y == y-1) || (rnd_x == x-1 && rnd_y == y) || (rnd_x == x-1 && rnd_y == y+1)
                || (rnd_x == x && rnd_y == y-1) || (rnd_x == x && rnd_y == y) || (rnd_x == x && rnd_y == y+1)
                || (rnd_x == x+1 && rnd_y == y-1) || (rnd_x == x+1 && rnd_y == y) || (rnd_x == x+1 && rnd_y == y+1));

        if((cells + rnd_x*rows + rnd_y)->value != 9)
        {
            (cells + rnd_x*rows + rnd_y)->value = 9;

            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if((cells + (rnd_x+j)*rows + (rnd_y+i))->value != 9 && rnd_x+j < cols && rnd_x+j >= 0 && rnd_y+i < rows && rnd_y+i >= 0)
                        (cells + (rnd_x+j)*rows + (rnd_y+i))->value += 1;

            mines--;
        }
    }
}

void display_board(int rows, int cols, cell* cells)
{
    int y, x;

    clrscr();

    mines = 1+(cols*rows)/10;
    if(timer_start){
        time(&end_t);
        diff_t=difftime(end_t+prev_diff_t, start_t);
    }

    printf("moves : %d;  F : %d;  ? : %d;  mines : %d; timer : %02d:%02d\n\n", moves, flags, questions, mines,mintues(),seconds());

    for(y=0; y<rows; y++)
    {
        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
        printf("%2d ",y+1);
        for(x=0; x<cols; x++)
        {


            if(lost && (cells + x*rows + y)->value == 9 && (cells + x*rows + y)->is_open == 0 && (cells + x*rows + y)->is_flagged == 0)
            {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 12);
                printf(" M ");
            }
            else if((cells + x*rows + y)->is_open == 1)
            {
                if((cells + x*rows + y)->value == 9)
                {
                    if(!win){
                        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_RED);
                    }
                    else
                        SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 047);
                    printf(" * ");
                }
                else if((cells + x*rows + y)->value != 0)
                {
                    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 121);
                    printf(" %d ",(cells + x*rows + y)->value);
                }
                else
                {
                    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 249);
                    printf(" %d ",(cells + x*rows + y)->value);
                }
            }
            else if((cells + x*rows + y)->is_flagged == 1)
            {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 237);
                if(lost && (cells + x*rows + y)->is_flagged != 9)
                    printf(" - ");
                else
                    printf(" F ");
            }
            else if((cells + x*rows + y)->is_question == 1)
            {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 175);
                printf(" ? ");
            }
            else
            {
                SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_BLUE);
                printf(" X ");
            }

        }
        printf("\n");
    }
    SetConsoleTextAttribute (GetStdHandle(STD_OUTPUT_HANDLE), 0x07);
    printf("   ");
    for(x=0; x<cols; x++)
    {
        printf("%2d ",x+1);
    }
    printf("\n");
}

void open_cell(int x, int y, char action, int rows, int cols, cell* cells)
{
    int i, j;

    if(board_created != 1)
    {
        if(action != 'f' && action != 'q')
        {
            create_board(x, y, rows, cols, (cell*)cells);
            board_created = 1;
        }
    }

    if(action == 'q' && (cells + x*rows + y)->is_open == 0 && (cells + x*rows + y)->is_question == 0)
    {
        (cells + x*rows + y)->is_question = 1;
        if((cells + x*rows + y)->is_flagged == 1)
        {
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

    if(!first_move)
    {
        if((cells + x*rows + y)->is_open == 1)
        {
            //if((cells + x*rows + y)->value != 0){
            if((cells + x*rows + y)->flags >= (cells + x*rows + y)->value && action != '-')
                for(j = -1; j <= 1; j++)
                    for(i = -1; i <= 1; i++)
                        if(x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                            if((cells + (x+j)*rows + y+i)->is_open != 1)
                            {
                                open_cell(x+j,y+i, '-',rows,cols,cells);
                                /*if((cells + (x+j)*rows + y+i)->is_flagged != 1)
                                    old_moves++;*/
                            }
            //}
        }
    }

    /*if(old_moves != 0)
    {
        moves++;
    }*/

    if(action == 'f' || action == 'q' || (cells + x*rows + y)->is_flagged == 1 || (cells + x*rows + y)->is_question == 1 || (cells + x*rows + y)->is_open == 1 )
        return;
    else
    {
        (cells + x*rows + y)->is_open = 1;
        if((cells + x*rows + y)->value == 0)
        {
            for(j = -1; j <= 1; j++)
                for(i = -1; i <= 1; i++)
                    if(x+j < cols && x+j >= 0 && y+i < rows && y+i >= 0)
                        open_cell(x+j,y+i, 'o',rows,cols,cells);
        }
        else if((cells + x*rows + y)->value == 9)
        {
            lost = 1;
        }
    }
}

int check_win(int rows, int cols, cell* cells)
{
    int x, y, cells_left = 0;

    for(y=0; y<rows; y++)
    {
        for(x=0; x<cols; x++)
        {
            if((cells + x*rows + y)->is_open == 0)
            {
                cells_left++;
            }
        }
    }

    if(current_left_cells != cells_left) {
        moves++;
        current_left_cells = cells_left;
    }

    if(cells_left == mines)
    {
        for(y=0; y<rows; y++)
        {
            for(x=0; x<cols; x++)
            {
                if((cells + x*rows + y)->is_open == 0)
                {
                    (cells + x*rows + y)->is_open = 1;
                }
            }
        }
        return 1;
    }

    return 0;
}

void newGameSettings(int rows, int cols,cell* cells)
{
    int y,x;
    board_created = 0, lost = 0, win = 0, first_move = 1, moves = 0, current_left_cells = 0, flags = 0, questions = 0, mines = 0, timer_start = 0, diff_t = 0, prev_diff_t = 0;
    for(y=0; y<rows; y++)
    {
        for(x=0; x<cols; x++)
        {
            (cells + x*rows + y)->value = 0;
            (cells + x*rows + y)->flags = 0;
            (cells + x*rows + y)->is_open = 0;
            (cells + x*rows + y)->is_flagged = 0;
            (cells + x*rows + y)->is_question = 0;
        }
    }
}

void PlayG(int cols,int rows,int is_load)
{

    int x, y, x_mv, y_mv, input_exist = 1, valid_input = 1;
    char action;
    char name[10],tempstr[10],buffer[15]="";
    int save_success = 0;

    if(is_load)
        loadMainSettings(&rows,&cols);

    cell cells[cols][rows];

    if(is_load)
    {
        loadArray((cell*)cells,rows,cols);
    }
    else{
        newGameSettings(rows,cols,(cell*)cells);
        current_left_cells = cols*rows;
    }

    display_board(rows, cols, (cell*)cells);



    while(!lost && !win)
    {


        if(!win)
        {

            if(!input_exist || !valid_input)
            {
                input_exist = 1;
                valid_input = 1;
                display_board(rows, cols, (cell*)cells);
                if(save_success == 1)
                {
                    printf("Game saved successfully!\n");
                    save_success = 0;
                }
                else
                    printf("Enter a VALID move!\n");
            }

            printf("Enter Your Move : X Y Action\n");
            input_exist = input(buffer,15,1);
            if(input_exist)
            {
                valid_input = validateMove(buffer);
                if(valid_input)
                {
                    sscanf(buffer,"%d %d %c", &x_mv, &y_mv, &action);

                    /*if(action != 'q' && action != 'f' && cells[x_mv-1][y_mv-1].is_open == 0)
                        moves++moves++;*/
                    if(!(x_mv < 1 || x_mv > cols || y_mv < 1 || y_mv > rows)){
                        open_cell(x_mv-1, y_mv-1, action, rows, cols, (cell*)cells);
                        if(!timer_start){
                            time(&start_t);
                            timer_start = 1;
                            diff_t=difftime(end_t+prev_diff_t, start_t);
                        }
                    } else {
                        valid_input = 0;
                        continue;
                    }
                }
                else
                {
                    if(strcmp(buffer,"menu") == 0)
                    {
                        break;
                    }
                    else if(strcmp(buffer,"save") == 0)
                    {
                        time(&end_t);
                        diff_t=difftime(end_t+prev_diff_t, start_t);
                        save_success = 1;
                        saveData((cell*)cells,rows,cols);
                    }
                }
            }
            else continue;
        }

        int wn = check_win(rows, cols, (cell*)cells);
        display_board(rows, cols, (cell*)cells);

        if(first_move && action != 'f')
        {
            first_move = 0;
        }

        if(lost)
        {
            display_board(rows,cols,(cell*)cells);
            printf("\nYOU LOST THE GAME !");
            printf("\nPress ' Enter ' to get back\n");
            fflush(stdin);
            gets(tempstr);
            continue;
        }

        if(wn)
        {
            win = 1;
            time(&end_t);
            display_board(rows, cols, (cell*)cells);
            printf("Congratulations!You Won\n");
            diff_t=difftime(end_t+prev_diff_t, start_t);
            leaderboard winner;
            winner.score=(pow(rows,4)*pow(cols,4))/((moves)*diff_t);
            printf("Your Score is : %d\n",winner.score);
            printf("Enter your name:\n");
            scanf("%s",winner.player);
            add_winner(winner);
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
                PlayG(4,4,0);
            }
            if(ch1==50)
            {
                PlayG(6,6,0);
            }
            if(ch1==51)
            {
                PlayG(8,8,0);
            }
            if(ch1==52)
            {
                int rows,cols,input_exist=1,valid_input=1;
                char buffer[15];
                system("cls");
                srand(time(NULL));
                while(1)
                {
                    if(!input_exist || !valid_input)
                    {
                        system("cls");
                        printf("Enter a VALID board!\n");
                    }

                    printf("Enter rows(4 - 30) & columns(4 - 24):\n");
                    input_exist = input(buffer,15,1);
                    if(input_exist)
                    {
                        valid_input = validateBoard(buffer);
                        if(valid_input)
                        {
                            sscanf(buffer,"%d %d", &rows, &cols);
                            if(!(rows < 4 || cols < 4 || rows > 30 || cols > 24)) {
                                PlayG(rows,cols,0);
                                break;
                            } else {
                                valid_input = 0;
                                continue;
                            }
                        }
                        else continue;
                    }
                    else continue;
                }
            }
        }
        if(ch==50)
        {
            system("cls");
            if(!display_leader_board())
                printf("\nThere are no winners yet!");
            printf("\nPress ' Enter ' to get back\n");
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
            PlayG(0,0,1);
            continue;
        }
    }
}

void clrscr()
{
    system("@cls||clear");
}

int input(char *string,int length,int required)
{
    int x = 0;

    fgets(string,length,stdin);
    if(*string != '\n' && strcmp(string,"-\n") != 0)
    {
        while(*string)
        {
            if(*string == '\n')
            {
                *string = '\0';
                return 1;
            }
            x++;
            if(x == length)
            {
                *string = '\0';
                return 1;
            }
            string++;
        }
        scanf("%*[^\n]");
        scanf("%*c");//clear upto newline
    }
    else if(required)
    {
        strcpy(string,"-");
    }
    else
    {
        strcpy(string,"-");
    }
    return 0;
}

int validateMove(char buffer[])
{

    int x,y;
    char action;
    int c1 = 0;

    c1 = sscanf(buffer,"%d %d %c",&x,&y,&action);


    if(c1!=3)
    {
        return 0;
    }

    return 1;
}

int validateBoard(char buffer[])
{

    int x,y;
    int c1 = 0;

    c1 = sscanf(buffer,"%d %d",&x,&y);


    if(c1!=2)
    {
        return 0;
    }

    return 1;
}


void saveData(cell* cells,int rows,int cols)
{
    FILE *f1;
    int y,x;
    f1=fopen("savedata.txt","w");

    if(f1!=NULL)
    {
        fprintf(f1,"%d %d %d %d %d %d %d %d %d %d %d %d\n",board_created, lost, win, first_move, moves, flags, questions, mines,rows,cols,current_left_cells,diff_t);

        fclose(f1);
        f1=fopen("savedata.txt","a");
        for(y=0; y<rows; y++)
        {
            for(x=0; x<cols; x++)
            {
                fprintf(f1,"%d %d %d %d %d\n",(cells + x*rows + y)->value,(cells + x*rows + y)->flags,(cells + x*rows + y)->is_open,(cells + x*rows + y)->is_flagged,(cells + x*rows + y)->is_question);
            }
        }
    }
    fclose(f1);

}

void loadMainSettings(int* rows,int* cols)
{
    FILE *f2;
    int y,x;
    f2=fopen("savedata.txt","r");

    if(f2!=NULL)
    {
        fscanf(f2,"%d %d %d %d %d %d %d %d %d %d %d %d\n",&board_created, &lost, &win, &first_move, &moves, &flags, &questions, &mines,rows,cols,&current_left_cells,&diff_t);
    }
    fclose(f2);

    prev_diff_t = diff_t;
    timer_start = 0;
}


void loadArray(cell* cells,int rows,int cols)
{
    FILE *f2;
    int y,x;
    f2=fopen("savedata.txt","r");

    if(f2!=NULL)
    {
        fscanf(f2,"%d %d %d %d %d %d %d %d %d %d %d %d\n",&board_created, &lost, &win, &first_move, &moves, &flags, &questions, &mines,&rows,&cols,&current_left_cells,&diff_t);
        for(y=0; y<rows; y++)
        {
            for(x=0; x<cols; x++)
            {
                fscanf(f2,"%d %d %d %d %d\n",&(cells + x*rows + y)->value,&(cells + x*rows + y)->flags,&(cells + x*rows + y)->is_open,&(cells + x*rows + y)->is_flagged,&(cells + x*rows + y)->is_question);
            }
        }
    }
    fclose(f2);

}

int seconds() {
    int mins = diff_t/60;
    return diff_t - (mins*60);
}

int mintues() {
    return diff_t/60;
}
