//
// Created by Hazannovich on 025, 25/Feb/2022.
//

#ifndef TIMER_DOT_H
#define TIMER_DOT_H
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#define ONE 49
#define TWO 50
#define THREE 51
#define FOUR 52
void intro();
void load_file(FILE **);
int found_date(char *, int, int, int);
void get_token(char *, char *);
int found_empty_line(char *);
void initializeCurses();
int set_work_time();
int set_break_time();
int work_countdown(int *);
int break_countdown(int *, int);
void print_play_work(int, int);
void print_pause_work(int, int);
void print_play_break(int, int, int);
void print_pause_break(int, int, int);
void what_next(int, int, int, int, FILE *, int *);
#endif // TIMER_DOT_H
