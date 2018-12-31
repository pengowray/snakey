#include <stdio.h>
#include <DOS.H>
#include <CONIO.H>
#include <alloc.h>
#include <stdlib.h>
#include <time.h>
#include <BIOS.H>

void main (void) {

int x=0;
unsigned short old, noo = 1;

 while (!kbhit()) {
   asm mov ah,0x00;
   asm int 0x1a;
   asm mov noo, dx;
   printf("%d, %lu\n",x, noo);
   x++;
 }



}