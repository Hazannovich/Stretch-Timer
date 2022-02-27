//
// Created by Hazannovich on 025, 25/Feb/2022.
//
#include "timer.h"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc50-cpp"
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-msc51-cpp"


int main(){
    int counter;
    int seconds = 59;
    int user_input;
    int choose_exercise;
    int count_cycles = 0;
    int skip_break = 0;

    char *urls[]={"Start https://www.stretchclock.com/w10/stretch.html#id=57",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=97",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=107",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=110",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=194",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=213",
                  "Start https://www.stretchclock.com/w10/stretch.html#id=226"};
    printf("\n~~~~~~~~~~~~~~~Welcome to Stretch Timer App!~~~~~~~~~~~~~~~\n");
    printf("     Stretch Timer Will Direct You to a Random Stretch\n"
           "      Every Completed Cycle of Minutes of Your Choice.\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("Enter The Time You Would Like to Cycle Through (Minutes): ");
    if(scanf_s("%d", &user_input, sizeof(int)) != 1)
    {
        fflush(stdout);
        fprintf(stderr, "Error: Number of Minutes Format is Integer only\n");
        exit(EXIT_FAILURE);
    }

    fflush(stdout);
    counter = user_input;
    printf("\n");

    while(!kbhit()) {
        //**********************************************************************
        //**************************WORK SECTION********************************
        while (!kbhit() && counter != 0) {
          while(!kbhit() && seconds != -1) {
            system("cls");
            printf("\n\n==================================================");
            printf("\n\tContinue to Work and Wait for the Beep\n");
            if(seconds >= 10)
              printf("\t\t     %d:%d Time Left",
                     counter -1, seconds);
            else
              printf("\t\t     %d:0%d Time Left",counter -1, seconds);
            printf("\n\t\tpress any key to leave");
            printf("\n==================================================");
            Sleep(1000);
            seconds--;
          }
          seconds = 59;
          counter--;
        }
        system("cls");
        //**********************************************************************
        //*************************BREAK SECTION********************************
        Beep(350, 1300);
        srand(time(NULL));
        choose_exercise = rand()%7;
        counter = 2;
        if(!kbhit())
          system(urls[choose_exercise]);
        while(!kbhit() && counter != 0) {
          while (!kbhit() && seconds != -1) {
            system("cls");
            printf("\n\n==================================================");
            printf("\n\t    Please Take Stretch Number %d...\n",
                   choose_exercise + 1);
            if (seconds >= 10)
              printf("\t\t     %d:%d Time Left", counter - 1, seconds);
            else
              printf("\t\t     %d:0%d Time Left", counter - 1, seconds);
            printf("\n\t\tpress any key to continue.");
            printf("\n==================================================");
            Sleep(800);
            seconds--;
          }
          if (kbhit())
            skip_break = 1;
          seconds = 59;
          counter--;
        }
        //in case user wants to skip the break,reset keyboard hit.
        if(skip_break){
          getch();
          skip_break = 0;
        }
        //Continue sound.
        if(!kbhit())
          Beep(1000, 1300);
        else{//Case for finishing this session.
          printf
              ("\n\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
          printf("\t    You have Completed %d cycles of %d Minutes.\n",
                 count_cycles, user_input);
          printf("     Thank You for Using Stretch Timer, See You Next "
                 "Time!\n");
          printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
          Sleep(2000);
        }
        counter = user_input;
        count_cycles++;
    }

    return 0;
}
