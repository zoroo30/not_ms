#ifndef LEADERBOARD_H_INCLUDED
#define LEADERBOARD_H_INCLUDED

typedef struct
{
    char player[20];
    int score;
} leaderboard;

extern leaderboard leaders[10];
#endif // LEADERBOARD_H_INCLUDED
