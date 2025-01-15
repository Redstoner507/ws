/*
==========================================
    ws.c: WS version 2.0
	Copyright 2024
                 Stoll-Geyer Yann
	Last Modified: 17/11/2024
==========================================
*/

#include <curses.h>
#include <signal.h>
#include <unistd.h>
#include "ws.h"

void add_smoke(int y, int x);
int add_sws(int x);
int add_BB6(int x);
int add_BBA(int x);
void option(char *str);
int my_mvaddstr(int y, int x, char *str);

int SWS  = 0;
int BB64 = 0;


int my_mvaddstr(int y, int x, char *str){
    for ( ; x < 0; ++x, ++str)
        if (*str == '\0')  return ERR;
    for ( ; *str != '\0'; ++str, ++x)
        if (mvaddch(y, x, *str) == ERR)  return ERR;
    return OK;
}

void option(char *str){
    extern int SWS,BB64;

    while (*str != '\0') {
        switch (*str++) {
            case 'l': SWS = 1; break;
            case 'W': BB64 = 1; break;
            default: break;
        }
    }
}

int main(int argc, char *argv[]){
    int x,i; 
    for (i = 1; i < argc; ++i) {
        if (*argv[i] == '-') {
            option(argv[i] + 1);
        }
    }

    initscr(); // Initialise  la lib (curses.h)
    signal(SIGINT, SIG_IGN); // Retire l'effet du Ctrl+C
    noecho(); // Empêche l'affichage des caractères entrée
    curs_set(0); // Rend le curseur invisible
    nodelay(stdscr, TRUE);
    leaveok(stdscr, TRUE);
    scrollok(stdscr, FALSE); // Empêche le Scroll

    for(x = COLS - 1; ; --x){
        if(SWS == 1){
            if (add_sws(x) == ERR) break;
        }
        else if(BB64 == 1){
            if (add_BB6(x) == ERR) break;
        }   
        else{
            if (add_BBA(x) == ERR) break;
        } 
        getch();
        refresh();
        usleep(40000);
        //clear(); // "Temporaire différent de sl" Nettoie l'écran
    }
    mvcur(0, COLS - 1, LINES - 1, 0);
    endwin(); // Nettoie et ferme la lib (curses.h)
    return 0;
}

int add_sws(int x){
    static char *sws[SWSHEIGHT+1] = {SWSSTR1, SWSSTR2, SWSDEL};

    int y, i;
    if (x < - SWSLENGTH)  return ERR;
    y = LINES / 2 - 5;

    for (i = 0; i <= SWSHEIGHT; ++i) {
        my_mvaddstr(y + i, x, sws[i]);     
    }

    return OK;
}

int add_BB6(int x){
    static char *BB6[BB6HEIGHT+1] = {BB6STR1,BB6STR2,BB6STR3,BB6STR4,BB6STR5,BB6STR6,
    BB6STR7,BB6STR8,BB6STR9,BB6STR10,BB6STR11,BB6STR12,BB6STR13,BB6DEL};
        
    int y, i;
    if (x < - BB6LENGTH)  return ERR;
    y = LINES / 2 - 5;

    for (i = 0; i <= BB6HEIGHT; ++i) {
        my_mvaddstr(y + i, x, BB6[i]);     
    }
    add_smoke(y+4,x+49);
    return OK;
}

int add_BBA(int x){
    static char *BBA[BBAHEIGHT+1] = {BBASTR1,BBASTR2,BBASTR3,BBASTR4,BBASTR5,BBASTR6,BBASTR7,BBASTR8,BBASTR9,BBASTR10,
    BBASTR11,BBASTR12,BBASTR13,BBASTR14,BBASTR15,BBASTR16,BBASTR17,BBASTR18,BBASTR19,BBASTR20,
    BBASTR21,BBASTR22,BBASTR23,BBASTR24,BBASTR25,BBASTR26,BBASTR27,BBASTR28,BBASTR29,BBASTR30,
    BBASTR31,BBASTR32,BBASTR33,BBASTR34,BBADEL};

    int y, i;
    if (x < - BBALENGTH)  return ERR;
    y = LINES / 2 - 15;

    for (i = 0; i <= BBAHEIGHT; ++i) {
        my_mvaddstr(y + i, x, BBA[i]);     
    }

    add_smoke(y+13,x+249);

    return OK;
}

void add_smoke(int y, int x)
#define SMOKEPTNS        16
{
    static struct smokes {
        int y, x;
        int ptrn, kind;
    } S[1000];
    static int sum = 0;
    static char *Smoke[2][SMOKEPTNS]
        = {{"(   )", "(    )", "(    )", "(   )", "(  )",
            "(  )" , "( )"   , "( )"   , "()"   , "()"  ,
            "O"    , "O"     , "O"     , "O"    , "O"   ,
            " "                                          },
           {"(@@@)", "(@@@@)", "(@@@@)", "(@@@)", "(@@)",
            "(@@)" , "(@)"   , "(@)"   , "@@"   , "@@"  ,
            "@"    , "@"     , "@"     , "@"    , "@"   ,
            " "                                          }};
    static char *Eraser[SMOKEPTNS]
        =  {"     ", "      ", "      ", "     ", "    ",
            "    " , "   "   , "   "   , "  "   , "  "  ,
            " "    , " "     , " "     , " "    , " "   ,
            " "                                          };
    static int dy[SMOKEPTNS] = { 2,  1, 1, 1, 0, 0, 0, 0, 0, 0,
                                 0,  0, 0, 0, 0, 0             };
    static int dx[SMOKEPTNS] = {-2, -1, 0, 1, 1, 1, 1, 1, 2, 2,
                                 2,  2, 2, 3, 3, 3             };
    int i;

    if (x % 4 == 0) {
        for (i = 0; i < sum; ++i) {
            my_mvaddstr(S[i].y, S[i].x, Eraser[S[i].ptrn]);
            S[i].y    -= dy[S[i].ptrn];
            S[i].x    += dx[S[i].ptrn];
            S[i].ptrn += (S[i].ptrn < SMOKEPTNS - 1) ? 1 : 0;
            my_mvaddstr(S[i].y, S[i].x, Smoke[S[i].kind][S[i].ptrn]);
        }
        my_mvaddstr(y, x, Smoke[sum % 2][0]);
        S[sum].y = y;    S[sum].x = x;
        S[sum].ptrn = 0; S[sum].kind = sum % 2;
        sum ++;
    }
}