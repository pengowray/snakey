#define uc unsigned char
#define us unsigned short

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

void pset(us x, uc y, uc col, char far *p)
{
 p[(y<<8) + (y<<6) + x] = col;
}