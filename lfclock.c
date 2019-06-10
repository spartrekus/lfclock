
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>  
#include <time.h>

#define STMAX 2040
#define ESC "\033"

//Format text
#define RESET 		0
#define BRIGHT 		1
#define DIM			2
#define UNDERSCORE	3
#define BLINK		4
#define REVERSE		5
#define HIDDEN		6
//Foreground Colours (text)
#define F_BLACK 	30
#define F_RED		31
#define F_GREEN		32
#define F_YELLOW	33
#define F_BLUE		34
#define F_MAGENTA 	35
#define F_CYAN		36
#define F_WHITE		37
//Background Colours
#define B_BLACK 	40
#define B_RED		41
#define B_GREEN		42
#define B_YELLOW	44
#define B_BLUE		44
#define B_MAGENTA 	45
#define B_CYAN		46
#define B_WHITE		47

///  norm
#define KRED  "\x1B[31m"
#define KGRE  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KNRM  "\x1B[0m"
///  bright
#define KBGRE  "\x1B[92m"
#define KBYEL  "\x1B[93m"


#define home() 			printf(ESC "[H") //Move cursor to the indicated row, column (origin at 1,1)
#define clrscr()		printf(ESC "[2J") //clear the screen, move to (1,1)
#define gotoyx(y,x)		printf(ESC "[%d;%dH", y, x);
#define    visible_cursor() printf(ESC "[?251");
#define nonvisible_cursor() printf(ESC "[?251");
#define resetcolor() printf(ESC "[0m")
#define set_display_atrib(color) 	printf(ESC "[%dm",color)


int sx1, sx2, sy1, sy2 ;
int rows, cols ; 
int pansel = 1;
int autorefresh = 1;


void gfxframe( int y1, int x1, int y2, int x2 )
{
    int foo, fooy , foox ;
    foo = x1;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    {
        gotoyx( fooy , foo );  
        printf( "|" );
    }

    foo = x2;
    for( fooy = y1 ; fooy <= y2 ; fooy++) 
    {
         gotoyx( fooy , foo );  
         printf( "|" );
    }
    foo = y1;
    for( foox = x1 ; foox <= x2 ; foox++) 
    {
         gotoyx( foo , foox );  
         printf( "-" );
    }
    foo = y2;
    for( foox = x1 ; foox <= x2 ; foox++) 
    {
         gotoyx( foo , foox );  
         printf( "-" );
    }
}


static struct termios oldt;
void restore_terminal_settings(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void enable_waiting_for_enter(void)
{
    tcsetattr(0, TCSANOW, &oldt);  /* Apply saved settings */
}

void disable_waiting_for_enter(void)
{
    struct termios newt;

    /* Make terminal read 1 char at a time */
    tcgetattr(0, &oldt);  /* Save terminal settings */
    newt = oldt;  /* Init new settings */
    newt.c_lflag &= ~(ICANON | ECHO);  /* Change settings */
    tcsetattr(0, TCSANOW, &newt);  /* Apply settings */
    atexit(restore_terminal_settings); /* Make sure settings will be restored when program ends  */
}





int main()
{
  char string[STMAX];

  struct winsize w; // need ioctl and unistd 
  ioctl( STDOUT_FILENO, TIOCGWINSZ, &w );
  clrscr();
  home();
  sy2   = w.ws_row  - 10; 
  sx2   = w.ws_col  - 10;
  sy1   = 2; 
  sx1   = 2;
  rows = sy2; cols = sx2; 

  long t;
  struct tm *ltime;

  int foo;

  int ch , gameover ; 
  gameover = 0;
  while( gameover == 0 )
  {
      disable_waiting_for_enter();
      clrscr();
      home();
      //set_display_atrib(B_BLUE); set_display_atrib(F_YELLOW);
      //gotoyx(7,10);
      //puts(	"┌─────────┐\n" );
    
      //set_display_atrib(B_BLUE); set_display_atrib(F_WHITE);
      //printf( "=lfclock=\n" );

      time(&t);
      ltime=localtime(&t);
    
      gotoyx(sy1+3, sx1+5);
      //set_display_atrib(B_BLUE); set_display_atrib(F_WHITE);
      printf( "Console Clock\n" );
      gotoyx(sy1+4, sx1+5);
      printf( "[Date %04d/%02d/%02d]\n", 1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday );
    
      gotoyx(sy1+2,sx1+5);
      //set_display_atrib(B_BLUE); set_display_atrib(F_WHITE);
      //printf( "Clock\n" );
      printf( "=lfclock=\n" );
    
    
      gotoyx( sy1 + (sy2-sy1)/2, sx1 + (sx2-sx1)/2);
      //set_display_atrib(B_BLUE); set_display_atrib(F_WHITE);
      printf( "Clock\n" );

      printf("%s", KYEL);
      gotoyx( sy1 + (sy2-sy1)/2 +1 , sx1 + (sx2-sx1)/2);
      printf( "[TIME :%02d:%02d:%02d:]\n", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
      printf("%s", KNRM);


      printf("%s", KNRM);
      gotoyx( sy2-2 , sx1 + (sx2-sx1)/2);
      //printf( "[Date :%04d/%02d/%02d:]\n", 1900 + ltime->tm_year, ltime->tm_mon +1 , ltime->tm_mday );

      foo = 1;
      if ( foo == 25 )
      {
         printf("%s", KNRM);
         gotoyx( sy2-2, sx2-12);
         printf( "|KEY?|" );
         gotoyx( sy1+1, sx1+1 );
         printf( "%d,%d,%d,%d", sy1, sx1, sy2, sx2 );
         gfxframe( sy1, sx1, sy2, sx2 );
      }

      printf("%s", KGRE);
      gotoyx( sy1+8,sx1+5);
      gotoyx(sy2-2,sx1+5);
      printf( ":Clock:\n" );
      printf("%s", KNRM);
      printf("%s", KNRM);

      ch = 0; 
      if ( autorefresh == 1 ) 
      {
         //usleep( 20 * 10000 );
         //usleep( 1 * 10 * 20 * 10000 );   // 2sec
         usleep( 1 * 10 * 10 * 10000 );     // 1sec under bsd
         //else if ( autorefresh == 6 ) usleep( 3 * 10 * 20 * 10000 );   // 6sec
      }
      else
         ch = getchar();

      if      ( ch == 'L' )   sx2++;
      else if ( ch == 'H' )   sx2--;
      else if ( ch == 'J' )   sy2++;
      else if ( ch == 'K' )   sy2--;
      else if ( ch == 'j' )   sy1++;
      else if ( ch == 'k' )   sy1--;
      else if ( ch == 'h' )   sx1--;
      else if ( ch == 'l' )   sx1++;
      else if ( ch == 's' ) 
      {
         if ( autorefresh == 1 ) autorefresh = 0; else autorefresh = 1;
      }

  }
  resetcolor();
  return 0;
}




