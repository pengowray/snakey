// I think I'll write my own Graphics routines.

// Inspired by Ben, 4th June.

#include <ALLOC.H>
#include <stdio.H>
#include <DOS.H>
//#include "d:\snakey\draw!.h"
#include <CONIO.H>
#include <MATH.H>
#include <mem.h>
#include <alloc.h>
//#define ISIZE 5 // size of icons (in SNAKExxx.C too)

/*

 ù Icons °°°°°
	  ù Read Û°°
		  ù Database of icons
		  ù Import PCX or whatever
	  ù Draw
		  û Quick 4th June
		  û Speed up 12th June
		  û Removed Icons' size from struct 18th June
		  û New look icons (ICON6.C) 25th June
		  ù Save background
	  ù Combine with Snakey
	  ù Use Palettes
	  ù Multiple palettes
	  ù Colour cycling
	  ù transparency
	  ù Spheres and refraction
	  ù objects with textures

 ù Screen full of picture °°°°°
 ù Effects icons °°°°°

*/

#define ICON struct _ICON

struct _ICON {

//   unsigned char x; // X size
//   unsigned char y; // Y size
   unsigned char *data; // pointer
   unsigned char *pal; // palette
};

short rotate ( char *image, char angle, short ISIZE) //(in 90ø)
{
 short x, y;
 char *rot;

 if (!(rot = (char far *) malloc(ISIZE*ISIZE)))
 {
   fprintf(stderr,"\nCannot allocate memory to rotate icon\n");
   return -1;
 }

 for (y=0; y<ISIZE; y++) {
   for (x=0; x<ISIZE; x++) {
//	 rot[y*ISIZE+x] = image[(ISIZE-1-x)*ISIZE+(ISIZE-1-y)];
//	 rot[y*ISIZE+x] = image[x*ISIZE+y];

	 rot[y*ISIZE+x] = image[x*ISIZE+(ISIZE-y-1)];

   }
 }
 memmove(image,rot,ISIZE*ISIZE);

 free(rot);
 return 1;
}

short icon_get (ICON *image, short which, short lib, // fills in the details of an ICON
			   unsigned char fcol, unsigned char col, short ISIZE)
{

short c, cx, c3,cc;

short f;
short max = 0; // 4^2
short min = 31000; // 4^2

short bl = 255;
short n=0;

float g = 0, iy,ix;

//short col = ISIZE; //72; // colours
//short fcol = 32; // first

//short col = 15; //72; // colours
//short fcol = 16; // first

float pi = 3.141;


 if (!(image->data = (char far *) malloc(ISIZE*ISIZE)))
 {
   fprintf(stderr,"\nCannot allocate memory for icon\n");
   return -1;
 }
 if (lib==1) { //....
  cc = 0;

  for (c=0; c < ISIZE ; c++ ) {
	for (cx=0; cx < ISIZE ; cx++ ) {
	  if (sqrt(cx*cx+c*c) > ISIZE) {
		 f=0;
	  }
	  else {
		 f= 4;
	  }
	  image->data[cc++] = f;
	}
  }
 }
 if (lib==2) {
  lib-=2;
  bl = which;
  if (bl == 5) bl = 10; // flip around tube bits of body
  else if (bl == 10) bl = 5;

  // pass ONE
  max = 0;
  min = 1000;
  for (c=0; c < ISIZE ; c++ ) {
	for (cx=0; cx < ISIZE ; cx++ ) {
	  f = 1;
		 if (bl&1) { f*= ISIZE-cx; n++; }
		 if (bl&2) { f*= c; n++; }
		 if (bl&4) { f*= cx; n++; }
		 if (bl&8) { f*= ISIZE-c; n++; }

		 if (f>max) max = f;
		 if (f<min) min = f;

	}
  }
  if (max == min) max++;

  // pass TWO
  cc = 0;
  for (c=0; c < ISIZE ; c++ ) {
	for (cx=0; cx < ISIZE ; cx++ ) {
	  f = 1;
		 if (bl&1) { f*= ISIZE-cx; n++; }
		 if (bl&2) { f*= c; n++; }
		 if (bl&4) { f*= cx; n++; }
		 if (bl&8) { f*= ISIZE-c; n++; }

		 if (f) { // 0 is black.
		   f = (f*col/(max-min)) + fcol; // rainbow
		   // *71 = number of colours-1.  +32 = starting colour
		 }

	  image->data[cc++] = f;
	}
  }
 }

 else if (lib == 0 ) {

  for (c=0; c < ISIZE * ISIZE; c++ ) {
	//  image->data[c] = (c%which) + 1;
	image->data[c] = which;//(c%which) + which;
  }
 }

 else if (lib == 1) {
  for (c=0; c < ISIZE * ISIZE; c++ ) {
   image->data[c] = (c%which) + which;
  }
 }

 else if (lib==4) {
  bl = which;
  cc=0;
  for (c=0; c < ISIZE ; c++ ) {
  iy = c+.5;

   for (cx=0; cx < ISIZE ; cx++ ) {
   ix = cx+.5;
	  if (bl == 10) { // body
		 g = col*sin(pi*ix/ISIZE);
		 if (g) g += fcol;
	  }
	  else if (bl == 5) { // body
		 g = col*sin(pi*iy/ISIZE);
		 if (g) g += fcol;
	  }
	  else if (bl == 4 ||bl==1||bl==2||bl==8) { // tail (cone)
		 if ( abs(iy-ISIZE/2) > (ix-1)/2 ) g=0; // || cx==0

		 //else g = col*sin(pi*((iy-0.5*ISIZE)*ISIZE/iy*iy)-ISIZE/2);
		 else g = col*cos(pi*(iy-0.5*ISIZE)/ix);
		 //else g = 3;
		 if (g > col) g = 0;
		 if (g) g += fcol;
	  }
	  else if (bl == 9 ||bl==3||bl==6||bl==12) { // curve
		 if (ix*ix + iy*iy > ISIZE*ISIZE ) g=0;
		 else g = col*sin(pi*(sqrt(iy*iy + (ix*ix)))/ISIZE);
		 if (g) g += fcol;
	  }
	  else if (bl == 11 ||bl==7||bl==13||bl==14) { // semi circle (head)
		 if (ix*ix + (iy-ISIZE/2)*(iy-ISIZE/2) > (ISIZE/2)*(ISIZE/2)) g=0;
		 else g=col*cos(pi* (sqrt( (ix*ix) + pow(iy-0.5*ISIZE,2) ) ) / ISIZE );

		 if (g) g += fcol;
	  }
	  else {
		 g= which;
	  }


	if (g < 0) { printf("g: %f \n\n",g);
				 g = abs(g);
			   }

	image->data[cc++] = (char)g;
   }
  }
  if (bl==2) rotate ( image->data, 1, ISIZE);
  if (bl==1) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }
  if (bl==8) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }

  if (bl==3) rotate ( image->data, 1, ISIZE);
  if (bl==6) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }
  if (bl==12) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }

  if (bl==7) rotate ( image->data, 1, ISIZE);
  if (bl==14) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }
  if (bl==13) { rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			   rotate ( image->data, 1, ISIZE);
			 }

 }

 return 1;
}



void icon_draw (ICON *image,short x, short y, char far *p, short ISIZE) { // draws the icon at x,y

 unsigned short sx=ISIZE,sy=ISIZE,offset, cx,cy, ty;
 unsigned char *dat;

 dat = &image->data[0];

 //sx = image->x;
 //sy = image->y;

 offset = x + (y*320);
 ty = 0;

 for (cy=0; cy < sy; cy++) {
  for (cx=0; cx < sx; cx++) {
	// put in trasparency thing soon...
	p[ty+cx + offset] = *dat++;
  }
  ty += 320;
  //dy += sy;
 }

}


void icondraw2 (ICON *image,short x, short y, char far *p, short ISIZE) { // draws the icon at x,y

 unsigned short sx=ISIZE,sy=ISIZE,offset, cx,cy, ty;
 unsigned char *dat;

 dat = &image->data[0];

 //sx = image->x;
 //sy = image->y;

 offset = x + (y*320);
 ty = 0;

 for (cy=0; cy < sy; cy++) {
  for (cx=0; cx < sx; cx++) {
	if (*dat)
	  p[ty+cx + offset] = *dat;
	  dat++;

  }
  ty += 320;
  //dy += sy;
 }

}


void back_draw (char far *image,short x, short y, char far *p, short ISIZE) { // draws the icon at x,y

 unsigned short sx=ISIZE,sy=ISIZE,offset, cx,cy, ty;
// unsigned char *dat;

// dat = image[0];

 //sx = image->x;
 //sy = image->y;

 offset = x + (y*320);
 ty = 0;

 for (cy=0; cy < sy; cy++) {
  for (cx=0; cx < sx; cx++) {
	// put in trasparency thing soon...
	p[ty+cx + offset] = image[ty+cx + offset];
  }
  ty += 320;
  //dy += sy;
 }

}

/*
void main (void) { // for testing.. yeah.

unsigned short c;

int x = 0;
int y = 0;

char far *p;

ICON image[2];
init();

p = (char far *)MK_FP( 0xa000, 0 );

icon_get( &image[0] );


icon_draw( &image[0], x,y,p);

getch();

deinit();
}

*/
