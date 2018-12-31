// nibbles!!!!!

#include <stdio.h>
#include <DOS.H>
#include <alloc.h>
#include "d:\peter\bc\nibbles\draw!.h"
#include <mem.h>

int main (void)
{

struct WIND { // the window that gets passed
 char far *screen;
 int top;
 int bot;
 int lef;
 int rig;
};

/*struct BOARD { // the playing board.. (64x64)

unsigned char pce[64][64]; // the pieces.

};*/

char far *on_screen; // pointer to the screen (0xa000 thingy)
char far *off_screen; // pointer off the screen for working..
WIND win;

on_screen = (char far *)MK_FP( 0xa000, 0 ); // make pointer to screen.

if (!(off_screen = (char far *) malloc(64000))) { // 64k for off screen..
  fprintf(stderr,"\nCannot allocate memory\n");
  return 0;
}

memset( off_screen, 0, 64000 ); // clear off screen area.
memcpy( on_screen, off_screen, 64000 );  // move off_screen data on screen


return 1;

}

