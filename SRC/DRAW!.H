// Drawing commands.. simple.   25th Feb 95.. still working on it..

// Hope to have a simple graphics library i can use for anything..

// needs to be sent:
// * a pointer to the screen or screen buffer thing..
// screen co-ordinates, top, bottom, left, right.. (ints)

// To be used with nibbles.cpp when i get it working..... <Sigh>

struct _WIND { // the window that gets passed (declared in draw!.h)
 char far *screen;
 int top;
 int bot;
 int lef;
 int rig;
};

#define WIND struct _WIND

void pset(WIND *win, int x, int y, unsigned char col); // draw dot w/ checking
void psetn(WIND *win, int x, int y, unsigned char col); // draw dot w/o checking

void init( void ); // go into VGA
void deinit( void ); // out of VGA
void waitv(void); // wait for verticle retrace

#include "d:\peter\bc\nibbles\draw!.c"
