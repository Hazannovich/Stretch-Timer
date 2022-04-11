//
// Created by Hazannovich on 025, 25/Feb/2022.
//
#include "timer.h"

int kbhit(void)
{
  struct termios oldt, newt;
  int ch;
  int oldf;

  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;
  newt.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);
  oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

  ch = getchar();

  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
  fcntl(STDIN_FILENO, F_SETFL, oldf);

  if (ch != EOF)
  {
    ungetc(ch, stdin);
    return 1;
  }

  return 0;
}
int main()
{
  int work_time_input;
  int break_time_input;
  int choose_exercise;
  int count_cycles = 0;
  int total_time = 0;
  int key_press_work;
  int key_press_break;
  FILE *file_handler;
  char *urls[] = {"python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=57\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=97\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=107\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=110\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=194\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=213\"",
                  "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=226\""};
  intro();
  load_file(&file_handler);
  work_time_input = set_work_time();
  break_time_input = set_break_time();

  while (key_press_work != FOUR && key_press_break != FOUR)
  {
    key_press_break = 0;
    key_press_work = work_countdown(&work_time_input);
    system("clear");
    if (key_press_work != FOUR)
    {
      system("play sound95.wav");
      srand(time(NULL));
      choose_exercise = rand() % 7;
      system(urls[choose_exercise]);
      key_press_work = 0;
      key_press_break = break_countdown(&break_time_input, choose_exercise);
    }
    what_next(key_press_work, key_press_break, count_cycles,
              work_time_input, file_handler, &total_time);
    count_cycles++;
  }
  fclose(file_handler);
  return 0;
}

void intro()
{
  printf("\n\t\t~~~~~~~~~~~~~~~Welcome to Stretch Timer App!~~~~~~~~~~~~~~~\n");
  printf("\t\t     Stretch Timer Will Direct You to a Random Stretch\n"
         "\t\t      Every Completed Cycle of Minutes of Your Choice.\n");
  printf("\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
}
void load_file(FILE **file_handler)
{
  char name[32];
  char line[81];
  char token[81];
  int user_found = 0;
  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  if (!(*file_handler = fopen("sessions.txt", "a+")))
  {
    if (!(*file_handler = fopen("sessions.txt", "w+")))
    {
      fprintf(stderr, "ERROR! Unable to create sessions file\n");
      exit(EXIT_FAILURE);
    }
  }

  printf("\t\t\tPlease Insert UserName(MAX 31): ");
  scanf("%s", name);
  fflush(stdout);
  while (!user_found && fgets(line, 81, *file_handler))
  {
    if (line[0] == '!')
    {
      get_token(line, token);
      if (!strcmp(token, name))
        user_found = 1;
    }
  }
  fseek(*file_handler, 0, SEEK_END);
  if (user_found)
  {
    printf("\t\t\tWelcome Back %s\n", name);
    /*while(!found_empty_line(line) && !session_found && fgets(line, 82,
                                                              *file_handler)){
      get_token(line, token);
      if(!found_empty_line(line)&&found_date(token, tm.tm_year+1900,tm
                                                                                 .tm_mon+1,tm.tm_mday)){
        session_found = 1;
        get_token(line, token);
        val = atoi(token);
      }
    }*/

    fprintf(*file_handler, "\t%02d/%02d/%d   %02d:%02d   ", tm.tm_mday,
            tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
  }
  else
  {
    fprintf(*file_handler, "!%s\n", name);
    fprintf(*file_handler, "\t%02d/%02d/%d   %02d:%02d   ", tm.tm_mday,
            tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
    printf("\t\t\tWelcome %s,\n"
           "\t\t\tThank you for giving Stretch Timer a Chance!\n",
           name);
  }
  printf("\t\t\tSession Time: %02d/%02d/%d   %02d:%02d\n", tm.tm_mday,
         tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
}
int found_date(char *date, int curr_year, int curr_month, int curr_day)
{
  char *token;
  int val;
  token = strtok(date, "/");
  if ((val = atoi(token)) == curr_day)
  {
    token = strtok(date, "/");
    if ((val = atoi(token)) == curr_month)
    {
      token = strtok(date, "/");
      if ((val = atoi(token)) == curr_year)
        return 1;
    }
  }
  return 0;
}
void get_token(char *line, char *token)
{
  int i = 1;
  int j = 0;
  while (isspace(line[i]) && line[i] != '\0')
    i++;
  while (!isspace(line[i]) && line[i] != '\0')
  {
    token[j++] = line[i];
    line[i] = line[++i];
  }
  token[j] = '\0';
}
int found_empty_line(char *line)
{
  int i = 0;
  while (line[i] != '\0')
  {
    if (!isspace(line[i++]))
      return 0;
  }
  return 1;
}
int set_work_time()
{
  int work_time;
  printf("\t\t\tEnter Your Work Time(Minutes): ");
  if (scanf("%d", &work_time) != 1)
  {
    fflush(stdout);
    fprintf(stderr, "\t\tError: Number of Minutes Format is Integer only\n");
    exit(EXIT_FAILURE);
  }
  fflush(stdout);
  printf("\n");
  return work_time;
}
int set_break_time()
{
  int break_time;
  printf("\t\t\tEnter Your Break Time(Minutes): ");
  if (scanf("%d", &break_time) != 1)
  {
    fflush(stdout);
    printf("\n");
    fprintf(stderr, "\t\tError: Number of Minutes Format is Integer only\n");
    exit(EXIT_FAILURE);
  }
  fflush(stdout);
  printf("\n");
  return break_time;
}
int work_countdown(int *work_time_input)
{
  int seconds = 59;
  int key_press_work = 0;
  int counter = *work_time_input;
  while (key_press_work != TWO && key_press_work != FOUR && counter != 0)
  {
    while (key_press_work != TWO && key_press_work != FOUR && seconds != -1)
    {
      print_play_work(counter, seconds);
      sleep(1);
      seconds--;
      if (kbhit())
      {
        key_press_work = (int)getchar();
        if (key_press_work == ONE)
        {
          print_pause_work(counter, seconds);
          key_press_work = 0;
          while (key_press_work != ONE && key_press_work != TWO && key_press_work != THREE && key_press_work != FOUR)
            key_press_work = (int)getchar();
        }
        if (key_press_work == THREE)
        {
          print_pause_work(counter, seconds);
          printf("\n\t\t\t    ~~~~~~~Edit Mode~~~~~~~\n");
          *work_time_input = set_work_time();
          counter = *work_time_input;
          seconds = 59;
        }
      }
    }
    seconds = 59;
    counter--;
  }
  return key_press_work;
}
int break_countdown(int *break_time_input, int choose_exercise)
{
  int seconds = 59;
  int key_press_break = 0;
  int counter = *break_time_input;
  while (key_press_break != FOUR && key_press_break != TWO && !kbhit() && counter != 0)
  {
    while (key_press_break != FOUR && key_press_break != TWO &&
           !kbhit() && seconds != -1)
    {
      print_play_break(counter, seconds, choose_exercise);
      sleep(1);
      seconds--;
      if (kbhit())
      {
        key_press_break = (int)getchar();
        if (key_press_break == ONE)
        {
          print_pause_break(counter, seconds, choose_exercise);
          key_press_break = 0;
          while (key_press_break != ONE && key_press_break != TWO &&
                 key_press_break != FOUR)
            key_press_break = (int)getchar();
        }
        else if (key_press_break == THREE)
        {
          print_pause_break(counter, seconds, choose_exercise);
          printf("\n\t\t\t    ~~~~~~~~~Edit Mode~~~~~~~~~\n");
          *break_time_input = set_break_time();
          counter = *break_time_input;
          seconds = 59;
        }
      }
    }
    seconds = 59;
    counter--;
  }
  return key_press_break;
}
void print_play_work(int counter, int seconds)
{
  system("clear");
  printf("\n\n\t\t==================================================\n");
  printf("\n\t\t\t   Keep Working and Wait for the Beep.\n");
  printf("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
  printf("\n\t\t\t(1)" GRN "Play" NRM "/Pause  (2)Skip  (3)Edit  "
         "(4)Exit\n");
  printf("\n\t\t==================================================");
}
void print_pause_work(int counter, int seconds)
{
  system("clear");
  printf("\n\n\t\t==================================================\n");
  printf("\n\t\t\t   Keep Working and Wait for the Beep.\n");
  printf("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
  printf("\n\t\t\t(1)Play/" GRN "Pause" NRM "  (2)Skip  (3)Edit  "
         "(4)Exit\n");
  printf("\n\t\t==================================================");
}
void print_play_break(int counter, int seconds, int choose_exercise)
{
  system("clear");
  printf("\n\n\t\t==================================================\n");
  printf("\n\t\t\t    Please Take Stretch Number %d...\n",
         choose_exercise + 1);
  printf("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
  printf("\n\t\t\t(1)" GRN "Play" NRM "/Pause  (2)Skip  (3)Edit  "
         "(4)Exit\n");
  printf("\n\t\t==================================================");
}
void print_pause_break(int counter, int seconds, int choose_exercise)
{
  system("clear");
  printf("\n\n\t\t==================================================\n");
  printf("\n\t\t\t    Please Take Stretch Number %d...\n",
         choose_exercise + 1);
  printf("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
  printf("\n\t\t\t(1)Play/" GRN "Pause" NRM "  (2)Skip  (3)Edit  "
         "(4)Exit\n");
  printf("\n\t\t==================================================");
}
void what_next(int key_press_work, int key_press_break, int count_cycles,
               int work_time_input, FILE *file_handler, int *total_time)
{
  *total_time += work_time_input;
  if (key_press_work == FOUR || key_press_break == FOUR)
  {
    fprintf(file_handler, "Total: %d Minutes\n", *total_time);
    printf("\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    printf("\t\t   You have Completed %d cycles of %d Minutes.\n",
           count_cycles, work_time_input);
    printf("\t\t   Thank You for Using Stretch Timer, See You Next "
           "Time!\n");
    printf("\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    sleep(4);
  }
  else
    system("play sound98.wav");
}