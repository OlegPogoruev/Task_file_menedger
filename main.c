

#include <ncurses.h>
#include "main.h"

#include <malloc.h>
#include <dirent.h>


#define MAX_FILE_NAME 1024
#define MAX_CHAR_IN_FILE_NAME 60

char aray_file_name[MAX_FILE_NAME][MAX_CHAR_IN_FILE_NAME];


#define MAX_LEN_WAY 4096
char way_to_dir_left[MAX_LEN_WAY] = "/home";
char way_to_dir_right[MAX_LEN_WAY] = "/home";

char* way_to_dir_activ;





/*
#define LEN_ARAY_STRING 1024
#define LEN_ARAY_CHAR 8192

#define OFSET_X 1
#define OFSET_Y 1
*/
void init_windows(void);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void refreshWindows(void);



void scrollUp(void);    /* Scrolls Up */
void scrollDown(void);  /* Scrolls Down */

int selection = 0;  // Index of currently selected item in `char* directories`
int start = 0;      // Index to start printing from `directories` array


void viem_file(void);
void change_way(void);
// Shows current directory left
WINDOW *win_left;

// Shows current directory right
WINDOW *win_right;


WINDOW *win_activ;


char choose_win = 0;


int len_win_aktiv = 0;
int len_win_left = 0;
int len_win_right = 0;


WINDOW *status_win;
// To store maximum height & width of terminal and store start x and y positions of windows
int /*startx, starty,*/ maxx, maxy;



int main()
{

    WINDOW * point_window;
//    int max_row, max_col;
    int c;
    
    point_window = initscr();


    for ( unsigned int i = 0; i < MAX_FILE_NAME; i++ )
    {
        for ( unsigned int j = 0; j < MAX_CHAR_IN_FILE_NAME; j++ )
        {
            aray_file_name[i][j] = 0;
        }
    }

    
    initscr();
    noecho();
    curs_set(0);
    start_color();

    // Get Size of terminal
    getmaxyx(stdscr, maxy, maxx);
    // Save last two rows for status_win
    maxy = maxy - 2;


    // Make the two windows side-by-side and make the status window
    init_windows();


    // Draw borders and refresh windows
    refreshWindows();

    wmove(win_activ,1,2);
    wprintw(win_activ, ">");
    viem_file();

    len_win_aktiv = len_win_left;
    way_to_dir_activ = way_to_dir_left;

//    move(0, 0);

    c = 0;

    while ( c != KEY_F(1))
    {

        c = wgetch(win_activ);
        switch ( c )
        {
            case KEY_UP:
                scrollUp();
            break;
            case KEY_DOWN:
                scrollDown();
            break;
            case KEY_LEFT:
                wmove(win_activ,selection+1,2);
                wprintw(win_activ, " ");
                refreshWindows();

                choose_win = 0;
                way_to_dir_activ = way_to_dir_left;
                win_activ = win_left;
                len_win_aktiv = len_win_left;

                selection = 0;
                wmove(win_activ,selection+1,2);
                wprintw(win_activ, ">");
            break;
            case KEY_RIGHT:
                wmove(win_activ,selection+1,2);
                wprintw(win_activ, " ");
                refreshWindows();

                choose_win = 1;
                way_to_dir_activ = way_to_dir_right;
                win_activ = win_right;
                len_win_aktiv = len_win_right;

                selection = 0;
                wmove(win_activ,selection+1,2);
                wprintw(win_activ, ">");
            break;
            case '\n':
                change_way();
                wclear(win_activ);
                viem_file();
                selection = 0;
                wmove(win_activ,selection+1,2);
                wprintw(win_activ, ">");
            break;

            default:
                
            break;
        }

    }



    endwin();
    return 0;
}

void change_way(void)
{
    int temp = 0;

if(0 == selection)
{
    for ( unsigned int i = 0; i < MAX_LEN_WAY; i++ )
    {
        if('/' == (*(way_to_dir_activ + i)))
        {
            temp = i;
        }
        if(0 == temp)
        {
            temp = 1;
        }
        if(0 == (*(way_to_dir_activ + i)))
        {
            break;
        }
    }

    for ( unsigned int i = temp; i < MAX_LEN_WAY; i++ )
    {
        (*(way_to_dir_activ + i)) = 0;
    }
}
else
{
    for ( unsigned int i = 0; i < MAX_LEN_WAY; i++ )
    {
        if(0 == (*(way_to_dir_activ + i)))
        {
            (*(way_to_dir_activ + i)) = '/';
    if (0 == choose_win)
    {
//        len_win_aktiv = len_win_left;
        strncpy ((way_to_dir_activ + i +1), &aray_file_name[selection - 1][0], MAX_CHAR_IN_FILE_NAME); 
    }
    else
    {
//        len_win_aktiv = len_win_right;
        strncpy ((way_to_dir_activ + i +1), &aray_file_name[(MAX_FILE_NAME/2) + selection - 1][0], MAX_CHAR_IN_FILE_NAME); 
    }
            break;
        }
    }
}




}


/*
   Creates current_win, preview_win and status_win
*/
void init_windows(void)
{
    win_left = create_newwin(maxy, maxx/2+2, 0, 0);
    win_right = create_newwin(maxy, maxx/2-1, 0, maxx/2+1);
    status_win = create_newwin(2, maxx, maxy, 0);
    keypad(win_left, TRUE);
    keypad(win_right, TRUE);
    win_activ = win_left;
}


/*
   Creates a new window with dimensions `height` and `width` starting at `starty` and `startx`
*/
WINDOW *create_newwin(int height, int width, int starty, int startx)
{
    WINDOW *local_win;
    local_win = newwin(height, width, starty, startx);
    return local_win;
}

/*
    Refresh ncurses windows
*/
void refreshWindows(void)
{
    box(win_left,0,0);
    box(win_right,0,0);

    wrefresh(win_left);
    wrefresh(win_right);
}





/*
    Scrolls Up
*/
void scrollUp(void)
{
        wmove(win_activ,selection+1,2);
        wprintw(win_activ, " ");
    selection--;
    selection = ( selection < 0 ) ? 0 : selection;

        wmove(win_activ,selection+1,2);
        wprintw(win_activ, ">");

    // Scrolling
/*
    if(len_win_aktiv >= maxy-1)
        if(selection <= start + maxy/2)
        {
            if(start == 0)
                wclear(win_activ);
            else
            {
                start--;
                wclear(win_activ);
            }
        }
*/
}


/*
    Scrolls Down
*/
void scrollDown(void)
{

    wmove(win_activ,selection+1,2);
    wprintw(win_activ, " ");

    selection++;
    selection = ( selection > len_win_aktiv-1 ) ? len_win_aktiv-1 : selection;

    wmove(win_activ,selection+1,2);
    wprintw(win_activ, ">");

    // Scrolling
/*
    if(len_win_aktiv >= maxy-1)
        if(selection - 1 > maxy/2)
        {
            if(start + maxy - 2 != len_win_aktiv)
            {
                start++;
                wclear(win_activ);
            }
        }
*/
}





//int len_win_aktiv = 0;
//int len_win_left = 0;
//int len_win_right = 0;
void viem_file(void)
{
    struct dirent **namelist;
    int n;
    int t = 1;
    int temp = 0;
    int temp2 = 0;

    n = scandir(way_to_dir_left, &namelist, NULL, alphasort);
    if (n == -1) {
        perror("scandir");
//        exit(EXIT_FAILURE);
    }

    len_win_left = 1;

/*
    if(0 == selection)
    {
        wmove(win_left,1,2);
        wprintw(win_left, ">");
    }
    else
    {
        wmove(win_left,1,2);
        wprintw(win_left, " ");
    }
*/
    wmove(win_left,t++,3);
    wprintw(win_left, "...");

    while (n--) {
        if('.' != namelist[n]->d_name[0])
        {

            ++len_win_left;
/*
            if(n == selection)
            {
                wmove(win_left,t,2);
                wprintw(win_left, ">");
            }
            else
            {
                wmove(win_left,t,2);
                wprintw(win_left, " ");
            }
*/

            wmove(win_left,t++,3);
//            wprintw(win_left, "%s\n", namelist[n]->d_name);
//            strncpy (&aray_file_name[temp][0], namelist[n]->d_name, MAX_CHAR_IN_FILE_NAME);
            if(((maxx/2) - 4) < MAX_CHAR_IN_FILE_NAME){temp2 = (maxx/2) - 4;}else{temp2 = MAX_CHAR_IN_FILE_NAME;}
            strncpy (&aray_file_name[temp][0], namelist[n]->d_name, temp2);
            wprintw(win_left, "%s\n", &aray_file_name[temp][0]);

            ++temp;

        }
        free(namelist[n]);
    }
    free(namelist);



    temp = MAX_FILE_NAME/2;
    t = 1;

    n = scandir(way_to_dir_right, &namelist, NULL, alphasort);
    if (n == -1) {
        perror("scandir");
//        exit(EXIT_FAILURE);
    }

    len_win_right = 1;
    wmove(win_right,t++,3);
    wprintw(win_right, "...");
    while (n--) {
        if('.' != namelist[n]->d_name[0])
        {
            ++len_win_right;
            wmove(win_right,t++,3);
//            wprintw(win_right, "%s\n", namelist[n]->d_name);
//            strncpy (&aray_file_name[temp][0], namelist[n]->d_name, MAX_CHAR_IN_FILE_NAME); 
            if(((maxx/2) - 4) < MAX_CHAR_IN_FILE_NAME){temp2 = (maxx/2) - 4;}else{temp2 = MAX_CHAR_IN_FILE_NAME;}
            strncpy (&aray_file_name[temp][0], namelist[n]->d_name, temp2);
            wprintw(win_right, "%s\n", &aray_file_name[temp][0]);
            ++temp;
        }
        free(namelist[n]);
    }
    free(namelist);


    if (0 == choose_win)
    {
        len_win_aktiv = len_win_left;
    }
    else
    {
        len_win_aktiv = len_win_right;
    }


refreshWindows();
}


