// Drawing commands.. simple.   25th Feb 95.. still working on it..

// Hope to have a simple graphics library i can use for anything..

// needs to be sent:
// * a pointer to the screen or screen buffer thing..
// screen co-ordinates, top, bottom, left, right.. (ints)

// To be used with nibbles.cpp when i get it working..... <Sigh>
/*
struct WIND { // the window that gets passed
 char far *screen;
 int top;
 int bot;
 int lef;
 int rig;
};
*/

#define WIND struct _WIND

void pset(WIND *win, int x, int y, unsigned char col) // draw dot w/ checking
{
  if ((unsigned)x < win->rig && (unsigned)x >= win->lef &&
      (unsigned)y >= win->top && (unsigned)y < win->bot ) {

     *(win->screen + (x+win->lef + (y+win->top)*320)) = col;
//   *(p + (x + ytable[y])) = col;
  }
}

void psetn(WIND *win, int x, int y, unsigned char col) // draw dot w/o checking
{
     *(win->screen + (x+win->lef + (y+win->top)*320)) = col;
//   *(p + (x + ytable[y])) = col;
}


void init( void ) // go into VGA
{
asm{
	mov ah, 0
	mov al, 0x13
	int 0x10
}
}



void deinit( void ) // out of VGA
{
  asm{
     mov ah, 0
     mov al, 3
     int 0x10
  }
}

void waitv(void) // wait for verticle retrace
{
 asm {
	push dx // store some stuff on the stack
	push ax //
	mov dx,0x3da  // move retrace to dx
 }
 wait:
 asm {
    in al,dx   // put dx in al
	test al,8 // check if the retrace thinggy is going on. 8=not start VR
    je wait   // if yes go to wait and start the loop again
    pop ax  // bring back the veribles
	pop dx
 }

}

