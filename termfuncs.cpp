#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

static const string CSI = "\033[";

//
// termfuncs.cpp -- some simple functions for using the terminal nicely
//
//   char getachar()     -- returns next char with no echo and no Enter needed
//   char getacharnow(ds)     -- only waits ds/10th seconds
//				 returns '\0' if no input by that time
//   void screen_clear() -- clears the screen
//   void screen_home()  -- moves cursor to top of screen
//   void screen_fg(string color)
//   void screen_bg(string color)
//   void screen_attr(string attr)
//   void screen_bright()
//   void screen_reset()
//   void place_cursor(int, int);   -- move the cursor to row col
//   void place_char(char, int, int)  -- move and place
//   void hide_cursor()
//   void show_cursor()
//
//   int  random_int(int low, int high)
//		returns random int in [low, high]
//		note: if SNAKE_SEED is set then use that for srand
//		      if not, then call srand with time(0) at first call
//   void seed_random(s)
//
// hist: 2014-09-30 added hide/show cursor and random_int
// hist: 2014-07-20 getachar now uses read, not cin >> c which skipped blanks
// hist: 2014-07-01 merged screendraw and termfuncs
//
static string color_names[] = { "black", "red", "green", "yellow",
				"blue",  "magenta", "cyan", "white" };
static const int num_colors = 8;

static string attr_names[] = { 	"reset", "bright", "dim", "",
				"underscore", "blink", "",
				"reverse", "hidden" };
static const int num_attrs = 9;

//
// getachar
//  returns a char using noecho and -icanon unless not a tty
//
char getachar()
{
    char c;
    
    if ( isatty(0) ){
        struct termios	info, orig;
        // system("stty -echo -icanon");
        tcgetattr(0, &info);
        orig = info;
        info.c_lflag &= ~ECHO;
        info.c_lflag &= ~ICANON;
        tcsetattr(0, TCSANOW, &info);
        
        read(0, &c, 1);
        
        // system("stty echo icanon");
        tcsetattr(0, TCSANOW, &orig);
    }
    else {
        read(0, &c, 1);
    }
    
    return c;
}
//
// getacharnow
//  returns a char using noecho and -icanon unless not a tty
//
char getacharnow(int decisecs)
{
    char c;
    
    if ( isatty(0) )
    {
        struct termios	info, orig;
        // system("stty -echo -icanon");
        tcgetattr(0, &info);
        orig = info;
        info.c_lflag &= ~ECHO;
        info.c_lflag &= ~ICANON;
        info.c_cc[VMIN] = 0;
        info.c_cc[VTIME] = decisecs;
        tcsetattr(0, TCSANOW, &info);
        
        if ( read(0, &c, 1) != 1 )
            c = '\0';
        
        tcsetattr(0, TCSANOW, &orig);
        // system("stty echo icanon");
    }
    else {
        if ( read(0, &c, 1) != 1 )
            c = '\0';
    }
    
    return c;
}

void screen_clear()
{
    cout << (CSI + "H");
    cout << (CSI + "2J");
}

void screen_home()
{
    cout << (CSI + "H");
}

void hide_cursor()
{
    cout << (CSI + "?25l");
}

void show_cursor()
{
    cout << (CSI + "?25h");
}
//
// lookup a string in an array
//   args: string to find, list of strings, len of list
//   rets: index of string or -1 for not found
//
static int lookup(string findme, string list[], int num)
{
    int	i;
    for(i=0;i<num;i++)
    {
        if ( list[i] == findme )
            return i;
    }
    return -1;
}
void screen_fg(string color)
{
    int	num = lookup(color, color_names, num_colors);
    if ( num >= 0 ){
        cout << CSI << ( 30 + num ) << "m";
    }
}
void screen_bg(string color)
{
    int	num = lookup(color, color_names, num_colors);
    if ( num >= 0 ){
        cout << CSI << ( 40 + num ) << "m";
    }
}
void screen_attr(string attr)
{
    int	num = lookup(attr, attr_names, num_attrs);
    if ( num >= 0 ){
        cout << CSI << num  << "m";
    }
}
void screen_bright()
{
    screen_attr("bright");
}
void screen_reset()
{
    screen_attr("reset");
}
void place_cursor(int row, int col)
{
    cout << "\033[" << (row+1) << ";" << (col+1) << "H";
    flush(cout);
}
void place_char(char c, int row, int col)
{
    place_cursor(row, col);
    cout << c;
    flush(cout);
}

static int rand_seed = -1;

void seed_random(int s)
{
    if ( s > 0 ){
        srand(s);
        rand_seed = s;
    }
}

int random_int(int lo, int hi)
{
    int	range = hi - lo + 1;
    char	*seed_str;
    
    if ( rand_seed == -1 ){
        seed_str = getenv( "SNAKE_SEED" );
        if ( seed_str != NULL )
            seed_random( (unsigned) atoi(seed_str) );
        else 
            seed_random( (unsigned) time(NULL) );
    }
    if ( range <= 0 )
        return 17;		// add to function specs
    
    return lo + ( rand() % range );
}
