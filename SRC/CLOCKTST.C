#include <stdio.h>
#include <DOS.H>
#include <CONIO.H>
#include <alloc.h>
#include <stdlib.h>
#include <time.h>
#include <BIOS.H>

void main (void) {

int x=0;
long old, noo;
old = clock();

 while (!kbhit()) {
   noo = clock();
   if (noo - old > CLK_TCK) {
	 printf("%d, %lu\n",x, noo-old);
	 x=0;
	 old = clock();
   }
   x++;
 }



}