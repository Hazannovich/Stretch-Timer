#include "timer.h"

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR)
    {
        ungetch(ch);
        return 1;
    }
    else
    {
        return 0;
    }
}
int main()
{
    char *urls[] = {"python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=57\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=97\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=107\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=110\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=194\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=213\"",
                    "python -m webbrowser \"https://www.stretchclock.com/w10/stretch.html#id=226\""};
    int work_time_input = 0;
    int break_time_input = 0;
    int choose_exercise = 0;
    int count_cycles = 0;
    int total_time = 0;
    int key_press_work = 0;
    int key_press_break = 0;
    FILE *file_handler = NULL;
    initializeCurses();
    start_color(); /* Start color 			*/
    init_pair(2, COLOR_MAGENTA, COLOR_BLACK);

    intro();
    load_file(&file_handler);
    work_time_input = set_work_time();
    break_time_input = set_break_time();
    nodelay(stdscr, TRUE);
    noecho();
    curs_set(0);
    while (key_press_work != FOUR && key_press_break != FOUR)
    {
        key_press_break = 0;
        key_press_work = work_countdown(&work_time_input);

        if (key_press_work != FOUR)
        {
            system("play sound95.wav");
            clear();
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
    getch();
    endwin();
    return 0;
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
            printw("\t\tERROR! Unable to create sessions file\n");
            exit(EXIT_FAILURE);
        }
    }

    printw("\t\t\tPlease Insert UserName(MAX 31): ");
    scanw("%s", name);
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
        printw("\t\t\tWelcome Back %s\n", name);
        /*while (!found_empty_line(line) && !session_found && fgets(line, 82, *file_handler))
        {
            get_token(line, token);
            if (!found_empty_line(line) && found_date(token, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday))
            {
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
        printw("\t\t\tWelcome %s,\n"
               "\t\t\tThank you for giving Stretch Timer a Chance!\n",
               name);
    }
    printw("\t\t\tSession Time: %02d/%02d/%d   %02d:%02d\n", tm.tm_mday,
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

void intro()
{
    printw("\n\t\t~~~~~~~~~~~~~~~Welcome to Stretch Timer App!~~~~~~~~~~~~~~~\n");
    printw("\t\t     Stretch Timer Will Direct You to a Random Stretch\n"
           "\t\t      Every Completed Cycle of Minutes of Your Choice.\n");
    printw("\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    refresh();
}
int set_work_time()
{
    int work_time;
    printw("\t\t\tEnter Your Work Time(Minutes): ");
    if (scanw("%d", &work_time) != 1)
    {
        fflush(stdout);
        printw("\n");
        printw("\t\tError: Number of Minutes Format is Integer only\n");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
    printw("\n");
    refresh();
    return work_time;
}
int set_break_time()
{
    int break_time;
    printw("\t\t\tEnter Your Break Time(Minutes): ");
    if (scanw("%d", &break_time) != 1)
    {
        fflush(stdout);
        printw("\n");
        printw("\t\tError: Number of Minutes Format is Integer only\n");
        exit(EXIT_FAILURE);
    }
    fflush(stdout);
    printw("\n");
    refresh();
    return break_time;
}

void initializeCurses()
{
    initscr(); // Start curses mode
    cbreak();
    keypad(stdscr, TRUE); // allow reading form keyboard
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
                key_press_work = getch();
                if (key_press_work == ONE)
                {
                    print_pause_work(counter, seconds);
                    key_press_work = 0;
                    while (key_press_work != ONE && key_press_work != TWO && key_press_work != THREE && key_press_work != FOUR)
                        if (kbhit())
                            key_press_work = getch();
                }
                if (key_press_work == THREE)
                {
                    print_pause_work(counter, seconds);
                    printw("\n\t\t\t    ~~~~~~~Edit Mode~~~~~~~\n");
                    nodelay(stdscr, FALSE);
                    echo();
                    curs_set(1);
                    *work_time_input = set_work_time();
                    nodelay(stdscr, TRUE);
                    noecho();
                    curs_set(0);
                    counter = *work_time_input;
                    seconds = 59;
                }
                flushinp();
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
    while (key_press_break != FOUR && key_press_break != TWO && counter != 0)
    {
        while (key_press_break != FOUR && key_press_break != TWO && seconds != -1)
        {
            print_play_break(counter, seconds, choose_exercise);
            sleep(1);
            seconds--;
            if (kbhit())
            {
                key_press_break = getch();
                if (key_press_break == ONE)
                {
                    print_pause_break(counter, seconds, choose_exercise);
                    key_press_break = 0;
                    while (key_press_break != ONE && key_press_break != TWO && key_press_break != THREE && key_press_break != FOUR)
                        if (kbhit())
                            key_press_break = getch();
                }
                if (key_press_break == THREE)
                {
                    print_pause_break(counter, seconds, choose_exercise);
                    printw("\n\t\t\t    ~~~~~~~~~Edit Mode~~~~~~~~~\n");
                    nodelay(stdscr, FALSE);
                    echo();
                    curs_set(1);
                    *break_time_input = set_break_time();
                    nodelay(stdscr, TRUE);
                    noecho();
                    curs_set(0);
                    counter = *break_time_input;
                    seconds = 59;
                }
                flushinp();
            }
        }
        seconds = 59;
        counter--;
    }
    return key_press_break;
}
void print_play_work(int counter, int seconds)
{
    erase();
    printw("\n\n\t\t==================================================\n");
    printw("\n\t\t\t   Keep Working and Wait for the Beep.\n");
    printw("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
    printw("\n\t\t\t(1)");
    attron(COLOR_PAIR(2));
    addstr("Play");
    attroff(COLOR_PAIR(2));
    printw("/Pause  (2)Skip  (3)Edit  (4)Exit\n");
    printw("\n\t\t==================================================");
    refresh();
}
void print_pause_work(int counter, int seconds)
{
    erase();
    printw("\n\n\t\t==================================================\n");
    printw("\n\t\t\t   Keep Working and Wait for the Beep.\n");
    printw("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
    printw("\n\t\t\t(1)Play/");
    attron(COLOR_PAIR(2));
    addstr("Pause");
    attroff(COLOR_PAIR(2));
    printw("  (2)Skip  (3)Edit  (4)Exit\n");
    printw("\n\t\t==================================================");
    refresh();
}
void print_play_break(int counter, int seconds, int choose_exercise)
{
    erase();
    printw("\n\n\t\t==================================================\n");
    printw("\n\t\t\t    Please Take Stretch Number %d...\n",
           choose_exercise + 1);
    printw("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
    printw("\n\t\t\t(1)");
    attron(COLOR_PAIR(2));
    addstr("Play");
    attroff(COLOR_PAIR(2));
    printw("/Pause  (2)Skip  (3)Edit  (4)Exit\n");
    printw("\n\t\t==================================================");
    refresh();
}
void print_pause_break(int counter, int seconds, int choose_exercise)
{
    erase();
    printw("\n\n\t\t==================================================\n");
    printw("\n\t\t\t    Please Take Stretch Number %d...\n",
           choose_exercise + 1);
    printw("\t\t\t\t     %d:%02d Time Left", counter - 1, seconds);
    printw("\n\t\t\t(1)Play/");
    attron(COLOR_PAIR(2));
    addstr("Pause");
    attroff(COLOR_PAIR(2));
    printw("  (2)Skip  (3)Edit  (4)Exit\n");
    printw("\n\t\t==================================================");
    refresh();
}
void what_next(int key_press_work, int key_press_break, int count_cycles,
               int work_time_input, FILE *file_handler, int *total_time)
{
    *total_time += work_time_input;
    if (key_press_work == FOUR || key_press_break == FOUR)
    {
        erase();
        fprintf(file_handler, "Total: %d Minutes\n", *total_time);
        printw("\n\n\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        printw("\t\t   You have Completed %d cycles of %d Minutes.\n",
               count_cycles, work_time_input);
        printw("\t\t   Thank You for Using Stretch Timer, See You Next "
               "Time!\n");
        printw("\t\t~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        refresh();
        sleep(4);
    }
    else
    {
        system("play sound98.wav");
        clear();
    }
}