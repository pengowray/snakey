// nibbles!!!!! started around 24th Febuary 1995
// worked before.. now adding better screen display.. and board etc.
//
// started Version 4 started 3rd May. Version 5 started 17th May
// √ Removing the annoying whr_beg() thing... █████ 17th may
// √ Movement buffer █████ 21st May
// √ Steal time thing █████ 17th May
// √ Set both X and Y of board size █████ 23rd May
// √ Improve time thing 14th June (10C)
// √ Put in crash detection █████ 22th May
// √ malloc the board █████ 24th May
// √ Renamed to "Snakey7.C" 24th May
// √ Icons ██ 8th June - crappy but they work! (now Snakey8.c)
// √ Two patterns.. one for each snake ██ 9th June.
// √ Wrap around screen + fixed draw screen bug (drew from 1,1) ██ 11th June
// √ Fixed illusion start bug ██ 12th June
// √ + and - speed up/down + quitting help ██ 12th June. 5th speed- 21st June
// √ Cursor key movement for player 2, = to see level.itm - June 22 (Snake11)
// ∙ Structure everything better.. use more subroutines ▒░░░░
// ∙ Board design (new boards) ░░░░░
// √ Food ██ 12th June
// √ Show's game speed + "Speed ups" 14th June (Snake10c)
// √ Game speed better.. 15th June (Snake10d)
// √ Proper icons for parts of snake and ISIZE argument ██ 21st June (Snake10F)
// √ Icon4.C now... With thingy... 21st June (Snake10F)
// √ Perfect icons & transparency and background - Snake11b June 24-25
// √ Wallpaper (wall.c) background - 26th June (Snake 11M)
// √ Reset and level_set() - 29th June (11N)
// √ Fixed screen wrapping bug thing - 3rd June (11n)
// ∙ More types of food ░░
// ∙ Power ups ░░░
// ∙ Enemies (plums) ░░░░░
// ∙ Computer player ░░░░░
// ∙ Better display screen ███░░
// ∙ Text on screen while playing... (Rott style)
// √ Include board.upd (where to update) █████ 12th June
// ∙ Title screen etc.. ░░░░░
// ∙ Config keys, Relative and Directional keys + Other configs ░░░░░
// ∙ Scrolling ░░░░░

// Current Main Areas of Concern: Menu, Board, Menu, Menu.

// And in the slightly distant future...

// ∙ Different board types.. Triangles or hex instead of squares..
// ∙ Modem support.. Flash Game? ░░░░░
// ∙ Better graphics.. Ray traced? ░░░░░
// ∙ Different players.. colours, names, abilities? █░░
// ∙ Glass, Metalic(T2), Stuffy, cord, <><>stuff, Allens, Real,
//   Scaley (sugar), Mud, trunk, pipes, tube, lizard, worm, hose,
//   /\/\/\/ stuff..
// ∙ Write docs... Info, Intructions,
// ∙ History of other snake games... ░░░░░
// ∙ Write bbs software to play it with... ░░░░░
// ∙ Make millions from it! $░░░░░░0.00
// ∙ Write a better multi player game :) ░░░░░

// ░Streety, ░Ultima, ░Astroids, ░Gunner, ░Pacman, ░Chess, ░Checkers,
// ░Atomic, ░Go, ░Car/Chariot, ░Gun game, ░Ball throwing, ░Moonbuggy,
// ░Tetris, ░Pool/Snooker, ░Pinball

/*
INT 1A - TIME - GET SYSTEM TIME
		AH = 00h
Return: CX:DX = number of clock ticks since midnight
		AL = midnight flag, nonzero if midnight passed since time last read
Notes:  there are approximately 18.2 clock ticks per second, 1800B0h per 24 hrs
		IBM and many clone BIOSes set the flag for AL rather than incrementing
		  it, leading to loss of a day if two consecutive midnights pass
		  without a request for the time (e.g. if the system is on but idle)
*/

//#define ISIZE 16 // size of icons (in ICONx.C too)

#include <stdio.h>
#include <DOS.H>
#include <CONIO.H>
#include <alloc.h>
#include <stdlib.h>
#include <time.h>
#include <mem.h>
#include "d:\snakey\draw!.h"
#include "d:\snakey\icon6.c"
#include "d:\snakey\wall!.c"

//#include "d:\snakey\icon\back.h"
//#include "d:\snakey\icon\snake1.h"
//#include "d:\snakey\icon\snake2.h"

//void _ExceptInit() {}

//#define BSIZEX 40 // size of board
//#define BSIZEY 25 // size of board
//#define BSIZEX 320/ISIZE // size of board
//#define BSIZEY 200/ISIZE // size of board

#define SLEN 200 // max size of snake -1
#define SNAKES 3 // Max number of snakes
#define BUF 3 // Max number of movements ahead stored

struct _SNAKE { // global
  unsigned short bitx[SLEN]; // snake position..  (x)
  unsigned short bity[SLEN]; // snake position..  (y)
  unsigned short siz; // size of snake +1
  unsigned short add; // amount to add to current length of snake
  char dir; // direction. (0 - 3)
  unsigned short startx; // starting position of snake
  unsigned short starty;
  //ICON *icon; // pointer to 1st icon of 20.. 0-15 body+tail. 16-20 head
  unsigned short icn;
  char crash; // crashed?
// |B|1|2|3|4|E| | | |
// SLEN = number of bytes in snake string.
};

#define SNAKE struct _SNAKE


void death( short c );
void add(unsigned char sna, unsigned char mov); // adding to buffer...
menu(void);
char reset(void);
char level_set(char h, char v);   // level reset


char movement[SNAKES][BUF]; // buffer of movements made to use
char ahead[SNAKES]; // current size of above array ( <= maxmov )
char maxmov = 2; // Number of moves ahead that can be made for each snake


enum directions { UP = 0, RIGHT, DOWN, LEFT };
char dirx[4] = {  0, 1, 0, -1 }; // direction for snake to move..
char diry[4] = {  -1, 0, 1, 0 }; //    "


#define BOARD struct _BOARD
struct _BOARD { // the playing board.. (64x64)
/*
  unsigned char pce[BSIZEY][BSIZEX]; // what pieces to put where.
  unsigned char typ[BSIZEY][BSIZEX]; // what they really mean.
  unsigned char itm[BSIZEY][BSIZEX]; // items on the board / snake
*/
  unsigned char *pce; // what pieces to put where.
  unsigned char *typ; // what they really mean.
  unsigned char *itm; // items/snake on the board
  unsigned char *icn; // how to draw them
  unsigned char *upd; // where on the board needs updating
};


int SPEED = 2; // number of steps each second

unsigned short offset; // offset for screen drawing
ICON nice[SNAKES*20 + 32*2]; // simple icons (40+64=104)
unsigned char col, fcol;
char drive; // speed counter for '+' and '-'
char rb = 0; // Show pieces and not icons? (toggle)

char vr = 1; // Wait for verticle retrace? (toggle)
char far *on_screen; // pointer to the screen (0xa000 thingy)
char far *off_screen; // pointer off the screen for working..
unsigned char *back; // background.

char newlevel;
char pause;

char grow; // amount food adds
char food; // food on the board?

unsigned char playing = 1; // are we playing the game?
unsigned char players = 2; // number of players
BOARD level; // stored as [y][x]
SNAKE plr[SNAKES]; // (two) snakes..
short ISIZE = 8, BSIZEX, BSIZEY;




main(int argc,char *argv[])  ///////////////////////////////////////
{


/*
struct WIND { // the window that gets passed (declared in draw!.h)
 char far *screen;
 int top;
 int bot;
 int lef;
 int rig;
};
*/

#define TICKS 250 // ticks before measuring

#define WIND struct _WIND

////               lef do ri up
//char dirx[4] = { -1, 0, 1, 0 }; // direction for snake to move..
//char diry[4] = {  0, 1, 0,-1 };
//enum directions { LEFT = 0, DOWN, RIGHT, UP };

//                up ri do lef



short c; // counter
short c2; // counter 2
unsigned short uc; // counter
float f; // temp float
char icn; // temp icn..
char key; // key pressed
short x,y,x2; // temp values
unsigned short tx[SNAKES], ty[SNAKES]; // ditto
unsigned char *dat, *dat2; // temp data pointer


//WIND win;
unsigned short dela, delay_count;
unsigned short clo;
unsigned short gamespeed, gamec, gameold;


randomize();

if (argc > 1) {
  ISIZE = atoi(argv[1]);
  if (ISIZE == 0) ISIZE = 8;
//  printf("ISIZE: %d, argv[2]: %s\n",ISIZE,argv[1]);
//  getch();
}

BSIZEX = 320/ISIZE; // size of board
BSIZEY = 200/ISIZE; // size of board

////

//dela = DX;

//INT 1A - TIME - GET SYSTEM TIME
//		AH = 00h
//Return: CX:DX = number of clock ticks since midnight




printf("Getting memory...\n");

on_screen = (char far *)MK_FP( 0xa000, 0 ); // make pointer to screen.

 if (!(level.pce = (char far *) malloc(BSIZEX*BSIZEY)) 		||
	 !(level.typ = (char far *) malloc(BSIZEX*BSIZEY)) 		||
	 !(level.itm = (char far *) malloc(BSIZEX*BSIZEY)) 		||
	 !(level.icn = (char far *) malloc(BSIZEX*BSIZEY)) 		||
	 !(level.upd = (char far *) malloc(BSIZEX*BSIZEY))		||
	 !(off_screen = (char far *) malloc(64000*sizeof(char))) ||
	 !(back = (char far *) malloc(64000*sizeof(char))) )
 {
   fprintf(stderr,"\nCannot allocate memory\n");
   return -1;
 }

printf("Clearing things...\n");
// CLEAR movement[][] and ahead[]
reset();

printf("Generating background...\n");


for (uc=0;uc<64000;uc++){
   // *(back+uc) = random(72)+32;
   *(back+uc) = random(2); //(char)uc; // black & blue noise
}


//wall(320,200,random(72),32,back);
//f = wall(320,200,random(72),32,back);
//printf("Zoom: %f\n",f);

// set up icons...
printf("Generating Icons (%dx%d)...\n",ISIZE,ISIZE);

plr[0].icn = 5;
plr[1].icn = 26;
//if ( icon_get(&nice[0], 15, 3, 200,8, ISIZE) < 0 ) return -1; //bground
if ( icon_get(&nice[0], 0, 0, 0, 0, ISIZE) < 0 ) return -1; //background
if ( icon_get(&nice[1], 2, 0, fcol,col, ISIZE) < 0 ) return -1; //snake1
if ( icon_get(&nice[2], 3, 0, fcol,col, ISIZE) < 0 ) return -1; //snake2
if ( icon_get(&nice[3], 15, 2, 55,4, ISIZE) < 0 ) return -1; //fruit
//if ( icon_get(&nice[3], 3, 1, 55,4, ISIZE) < 0 ) return -1; //fruit
if ( icon_get(&nice[4], ISIZE+1, 1, fcol,col, ISIZE) < 0 ) return -1; //wall

for (c=0; c <= 20; c++) {
  if ( icon_get(&nice[c+plr[0].icn], c, 4, 16,15, ISIZE) < 0 ) return -1;
  if ( icon_get(&nice[c+plr[1].icn], c, 4, 32,ISIZE, ISIZE) < 0 ) return -1;
}

// NEW ICONS!!!!!!!!!!!!! GET YOUR NEW ICONS!!!!!

/*
memcpy( nice[plr[0].icn + 11].data, head, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[0].icn +  5].data, body, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[0].icn +  6].data, turn, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[0].icn +  4].data, tail, ISIZE*ISIZE*sizeof(char) );

memcpy( nice[plr[1].icn + 11].data, head2, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[1].icn +  5].data, body2, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[1].icn +  6].data, turn2, ISIZE*ISIZE*sizeof(char) );
memcpy( nice[plr[1].icn +  4].data, tail2, ISIZE*ISIZE*sizeof(char) );
*/

//plr[0].icon = &nice[4];


//memset( off_screen, 0, 64000 ); // clear off screen area.
//memcpy( on_screen, off_screen, 64000 );  // move off_screen data on screen




printf("Setting up board and players...\n");

////// set up board and players....


////// set up snakes..


level_set(2,1);

//for (c=plr[0].icn; c < BSIZEX+plr[0].icn; c++) { // test pattern at the top
//  level.icn[c-plr[0].icn] = c;
//}

/*
icn=plr[1].icn; // Test pattern II - 4x4 grid
for (c=0; c < 4; c++){
  for (c2=0; c2 < 4; c2++){
	 level.icn[(c+2)*BSIZEX + c2+1] = icn++;
  }
}
*/

newlevel = 0;
pause = 1;

printf("Setting up timer and about to start...\n");

dela = SPEED; // delay in ticks

gamespeed = 0;
gamec     = 0;
gameold   = 0;
// end of setting up snakes.. (change later)

memset( off_screen, 0, 64000 ); // clear off screen area.
init(); // VGA mode!
memcpy( on_screen, off_screen, 64000 );  // move off_screen data on screen

asm mov ah,0x00; asm int 0x1a;
asm mov delay_count, dx;
delay_count += dela;

//////////////////////////// MAIN PLAYING LOOP /////////////////////////////
//deinit(); // for error checking

//CLK_TCK / SPEED;

while ( playing ) {


 asm mov ah,0x00; asm int 0x1a;
 asm mov clo, dx;

 if (gamec++ == 0) {
	gameold = clo;
 }



 while( kbhit() || clo < delay_count ) { // does continuously during a loop thing

  /// get keys and change direction of snake accordingly..

  if( kbhit() ){
   key = getch();
   // Should now just get, and stack(maybe) movements...
   //
   // 0 straight, 1 left, 2 right, 4-7 north, west...
   if (key == '1') { // move left.. (player 1)
	  add(1,0);
   }
   else if (key == '2') {
	  add(2,0);
   }
   else if (key == ',') {
	  add(1,1);
   }
   else if (key == '.') {
	  add(2,1);
   }
   else if (key == 27) {
	  //playing=0;
	  menu();
	  asm mov ah,0x00; asm int 0x1a;
	  asm mov delay_count, dx;
	  delay_count += dela;
	  //playing = 0;

   }
   else if (key == 0) {
	  key = getch();
	  if (key == 'H'){
		add(4,1); // UP
	  }
	  else if (key == 'M'){
		add(5,1); // RIGHT
	  }
	  else if (key == 'P'){
		add(6,1); // Down
	  }
	  else if (key == 'K'){
		add(7,1); // left
	  }
   }
   else if (key == '\r') {
	  memset( level.upd, 2, BSIZEX*BSIZEY ); // Why 2?! <shrug>
	  memcpy( on_screen, back, 64000);
	  getch();
   }
   else if (key == '+') {
	  drive++;
	  gamec=0;
		   if (drive == 1) { dela = SPEED;  vr = 1;} // delay in ticks
	  else if (drive == 2) { dela = 1;      vr = 1;}
	  else if (drive == 3) { dela = 0;      vr = 1;}
	  else if (drive == 4) { dela = 0;      vr = 0;}
	  else if (drive > 4) drive = 4;
   }
   else if (key == '-') {
	  drive--;
	  gamec=0;
		   if (drive == 0) { dela = 9;     vr = 1;}
	  else if (drive == 1) { dela = SPEED; vr = 1;}
	  else if (drive == 2) { dela = 1;     vr = 1;}
	  else if (drive == 3) { dela = 0;     vr = 1;}
	  else if (drive < 0) drive = 0;
   }
   else if (key == '*') { // wait for retrace toggle
	  gamec=0;
	  if (vr) vr = 0;
	  else vr = 1;
   }
   else if (key == '=') { // show real game pieces
	  if (rb) rb = 0;
	  else rb = 1;
   }

   else if (key == '\\') {
	 for (c = players-1; c >= 0; c--) {  // lengthen snakes
	   plr[c].add += 1;
	 }
   }
   else if (key == 'P' || key == 'p') {
	  pause=1;
   }

  }
  asm mov ah,0x00; asm int 0x1a;
  asm mov clo, dx;

 }

 asm mov ah,0x00; asm int 0x1a;
 asm mov delay_count, dx;
 delay_count += dela;


 for (c = players-1; c >= 0; c--) { // change direction of snakes...
   if (ahead[c]) {
	 if (movement[c][0] == 1) {
	   if (plr[c].dir) plr[c].dir--;
	   else plr[c].dir = 3;
	 }
	 else if (movement[c][0] == 2) {
	   if (plr[c].dir < 3) plr[c].dir++;
	   else plr[c].dir = 0;
	 }
	 else if (movement[c][0] > 2) {
	   if ((plr[c].dir+2)%4 != movement[c][0]-4)
	   plr[c].dir = movement[c][0]-4;
	 }
	 ahead[c]--;
	 memmove (&movement[c][0], &movement[c][1], sizeof(char)*ahead[c]);

   }
 }

 /////////// move snakes along..

 for (c = players-1; c >= 0; c--) { // take off tale bit (if needed)
	if (!plr[c].add || plr[c].siz+2 > SLEN) {
	  level.itm[ plr[c].bity[plr[c].siz] * BSIZEX + plr[c].bitx[plr[c].siz] ] = 0; // clear tail
	  level.icn[ plr[c].bity[plr[c].siz] * BSIZEX + plr[c].bitx[plr[c].siz] ] = 0; // clear tail from board
	  level.upd[ plr[c].bity[plr[c].siz] * BSIZEX + plr[c].bitx[plr[c].siz] ] = 1; // so it'll update that bit

	  plr[c].siz--;

	  if ( plr[c].bity[plr[c].siz]-1 == plr[c].bity[plr[c].siz-1] ) {
		 icn = 2; // ▀
	  }
	  else if ( plr[c].bitx[plr[c].siz]-1 == plr[c].bitx[plr[c].siz-1] ) {
		 icn = 1; // ▐
	  }
	  else if ( plr[c].bity[plr[c].siz]+1 == plr[c].bity[plr[c].siz-1] ) {
		 icn = 8; // ▄
	  }
	  else if ( plr[c].bitx[plr[c].siz]+1 == plr[c].bitx[plr[c].siz-1] ) {
		 icn = 4; // ▌
	  }
	  else if ( plr[c].bity[plr[c].siz] > plr[c].bity[plr[c].siz-1] ) {
		 icn = 8; // ▄
	  }
	  else if ( plr[c].bitx[plr[c].siz] > plr[c].bitx[plr[c].siz-1] ) {
		 icn = 4; // ▌
	  }
	  else if ( plr[c].bity[plr[c].siz] < plr[c].bity[plr[c].siz-1] ) {
		 icn = 2; // ▀
	  }
	  else if ( plr[c].bitx[plr[c].siz] < plr[c].bitx[plr[c].siz-1] ) {
		 icn = 1; // ▐
	  }
	  else icn = -5;

	  level.icn[ plr[c].bity[plr[c].siz] * BSIZEX + plr[c].bitx[plr[c].siz] ] = plr[c].icn+icn;
	  level.upd[ plr[c].bity[plr[c].siz] * BSIZEX + plr[c].bitx[plr[c].siz] ] = 1;
	}
	else {
	  plr[c].add--;
	}
 }

 //////// check for collisions..

 for (c = players-1; c >= 0; c--) {  // see where they'd go..
   tx[c] = plr[c].bitx[ 0 ] + dirx[ plr[c].dir ]; // see where x
   ty[c] = plr[c].bity[ 0 ] + diry[ plr[c].dir ]; // see where y

   // wrapping
   if (tx[c] == (unsigned short)(-1)) tx[c] = BSIZEX-1;
   else if (ty[c] == (unsigned short)(-1)) ty[c] =BSIZEY-1;
   else if (tx[c] >= BSIZEX) tx[c] = 0;
   else if (ty[c] >= BSIZEY) ty[c] = 0;
 }

 for (c = players-1; c >= 0; c--) {  // Did they crash? hmm...

	if (level.typ[ty[c]*BSIZEX + tx[c]]) { // crashed with board
	   plr[c].crash = 1;
	}
	if (level.itm[ty[c]*BSIZEX + tx[c]]) { // crashed with item
	 if (level.itm[ty[c]*BSIZEX + tx[c]] <= players) {  // (1 to players+1)
	   plr[c].crash = 1;
	 }
	 else if (level.itm[ty[c]*BSIZEX + tx[c]] == 4) { // wall!
	   plr[c].crash = 1;
	 }
	 else {
	   plr[c].add += grow;
	   food = 0;
	   grow++;
	   if (grow > 10) {
		  grow = 1; // go to next level
		  newlevel =1;
	   }
	 }
	}
	for (c2 = players-1; c2 >= 0; c2--) { // or with whom?
	 if (c2 != c && tx[c] == tx[c2] && ty[c] == ty[c2] ) {
	   plr[c].crash = 1;
	 }
	}

	if (plr[c].crash) {
	  death(c);
	  playing = 0;
	}
 }


 if (newlevel) {
  reset();
  level_set(random(3),random(3));
  newlevel=0;
  pause=1;
 }
 else {
  // add bit to front
  for (c = players-1; c >= 0; c--) { // add to snakes now..
	plr[c].siz++;
	memmove( &plr[c].bitx[1], &plr[c].bitx[0], sizeof(plr[0].bitx[0])*plr[c].siz); // move snake ->
	memmove( &plr[c].bity[1], &plr[c].bity[0], sizeof(plr[0].bitx[0])*plr[c].siz); // move snake ->
	plr[c].bitx[ 0 ] = tx[c];
	plr[c].bity[ 0 ] = ty[c];

	level.itm[ plr[c].bity[ 0 ]*BSIZEX + plr[c].bitx[ 0 ] ] = c+1; // add new head
	level.upd[ plr[c].bity[ 0 ]*BSIZEX + plr[c].bitx[ 0 ] ] = 2; // update
	level.icn[ plr[c].bity[ 0 ]*BSIZEX + plr[c].bitx[ 0 ] ] = // add new head icn
		plr[c].icn + (1<<(3-plr[c].dir)^15);

	// change body icon

	if (plr[c].siz > 1) {
	 icn=0;

		  if (plr[c].bitx[0]-1 == plr[c].bitx[1]) icn |= 4;
	 else if (plr[c].bitx[0]+1 == plr[c].bitx[1]) icn |= 1;
	 else if (plr[c].bity[0]-1 == plr[c].bity[1]) icn |= 2;
	 else if (plr[c].bity[0]+1 == plr[c].bity[1]) icn |= 8;
	 else if (plr[c].bitx[0] > plr[c].bitx[1]) icn |= 1;
	 else if (plr[c].bitx[0] < plr[c].bitx[1]) icn |= 4;
	 else if (plr[c].bity[0] > plr[c].bity[1]) icn |= 8;
	 else if (plr[c].bity[0] < plr[c].bity[1]) icn |= 2;


	if (icn != c - plr[c].icn) {
		  if (plr[c].bitx[2]-1 == plr[c].bitx[1]) icn |= 4;
	 else if (plr[c].bitx[2]+1 == plr[c].bitx[1]) icn |= 1;
	 else if (plr[c].bity[2]-1 == plr[c].bity[1]) icn |= 2;
	 else if (plr[c].bity[2]+1 == plr[c].bity[1]) icn |= 8;
	 else if (plr[c].bitx[2] > plr[c].bitx[1]) icn |= 1;
	 else if (plr[c].bitx[2] < plr[c].bitx[1]) icn |= 4;
	 else if (plr[c].bity[2] > plr[c].bity[1]) icn |= 8;
	 else if (plr[c].bity[2] < plr[c].bity[1]) icn |= 2;
	}
	 level.icn[ plr[c].bity[ 1 ]*BSIZEX + plr[c].bitx[ 1 ] ] = plr[c].icn+icn;
	}
  }
 }
 // add food
  if (food == 0) {
	c = 1;
	while (c) {
	  tx[0] = random(BSIZEX);
	  ty[0] = random(BSIZEY);

	  if (level.itm[ty[0]*BSIZEX + tx[0]] == 0 &&
		level.typ[ty[0]*BSIZEX + tx[0]] == 0) {

		level.itm[ ty[0]*BSIZEX + tx[0] ] = 3; // Food
		level.icn[ ty[0]*BSIZEX + tx[0] ] = 3; // Food

		level.upd[ ty[0]*BSIZEX + tx[0] ] = 1;

		food =1;
		c=0;
	  }
	}
  }

 // sped up board...

 if (rb) dat = &level.itm[0];
 else dat = &level.icn[0];

 dat2 = &level.upd[0];
 x=0; y=0;
 offset = 0; //x + (y*320)

 for (c = 0; c < BSIZEY; c++ ) { // y
  for (c2 = 0; c2 < BSIZEX; c2++ ) { // x
	if (*dat2) { // level.upd[y2+c2]
	  *dat2 -= 1;
	  if (*dat == 0){
		back_draw( back, x,y, off_screen, ISIZE );
		//icon_draw( &nice[*dat], x,y, off_screen, ISIZE );
	  }
	  else if (*dat < 4 ) { //plr[0].icn){
		back_draw( back, x,y, off_screen, ISIZE );
		icon_draw( &nice[*dat], x,y, off_screen, ISIZE );

	  }
	  else
		//icon_draw( &nice[*dat], x,y, off_screen, ISIZE );
		back_draw( back, x,y, off_screen, ISIZE );
		icondraw2( &nice[*dat], x,y, off_screen, ISIZE );
	}
	dat++;
	dat2++;
	x+=ISIZE;
	}
  x=0;
  y+=ISIZE;
  //y2+=BSIZEX;
 }

  if (vr) waitv();
  memcpy( on_screen, off_screen, 64000 );  // move off_screen data on screen

  if (pause) {
	gotoxy(15, 12); // screen = 40x25 text
	printf("PAUSED");
	getch();
	pause=0;
  }

// misc things...
//  memcpy ((on_screen + BSIZEY*320), &plr[0], sizeof(SNAKE) );
//  memcpy ((on_screen + (BSIZEY+2)*320), &plr[1], sizeof(SNAKE) );


// */
} //// end of main playing loop


asm mov ah,0x00; asm int 0x1a;
asm mov gamespeed, dx;

gamespeed -= gameold;

deinit(); // no more VGA mode! δ:)

 for (c = 0 ; c < players; c++) {  // print out losers:
   if (plr[c].crash) { printf("Player %d lost with a length of %d!\n", c+1, plr[c].siz); }
   else { printf("Player %d lived with a length of %d!\n", c+1, plr[c].siz); }
 }

printf("\nKeys for player 1: 1 2\n");
  printf("Keys for player 2: . ,\n");
  printf("'-' to slow down, '+' to speed up\n");
//  printf("'*' to toggle waiting for retrace\n");
  printf("Clock Ticks per screen: %f = %d / %d\n",(float)gamespeed/gamec, gamespeed, gamec);
printf("\nSnakey 11N (c) Peter Halasz 1995\n");
  printf("Please don't distribute too much\n");



return 1;

}


void death( short c ) {
  printf("Player %d has lost!\n",c+1);
//  getch();
}

void add(unsigned char mov, unsigned char sna) {

  if (ahead[sna] < maxmov) {
	movement[sna][ ahead[sna] ] = mov;
	ahead[sna]++;
  }

}

menu() {
char key;
char c;

memset( level.upd, 2, BSIZEX*BSIZEY ); // Why 2?! <shrug>
memcpy( on_screen, back, 64000);

gotoxy(1,1);
printf("1. Eat\n");
printf("2. Die\n");
printf("3. Get out of fridge\n");
printf("4. Set speed\n");
printf("5. Players\n");
printf("6. Icon size\n");
printf("7. New background\n");
key = getch();

	 if (key == '1') {
	   for (c = players-1; c >= 0; c--) { // Eat
		 plr[c].add += 6;
		 grow = 9;
	   }
	 }
else if (key == '2') { // Die
	   printf("You die\n");
	   reset();

	 }
else if (key == '3') { // Get out of fridge
	   playing = 0;
	 }
else if (key == '4') { // set speed
	   SPEED = 2; // number of steps each second
	   drive = 1; // speed counter for '+' and '-'
	   vr = 1; // Wait for verticle retrace? (toggle)
	   rb = 0; // Show pieces and not icons? (toggle)

	 }
else if (key == '5') { // players
	   players = 1;
	 }
else if (key == '6') { // set icon size
	   ISIZE = 8;
//	   BSIZEX = 320/ISIZE; // size of board
//	   BSIZEY = 200/ISIZE; // size of board

	 }
else if (key == '7') { // New background
	   wall(320,200,random(72),32,back);
	 }
return 1;

}


char reset()
{

char c;

	   col=15, fcol=16;
	   drive=1;
	   rb = 0;

	   grow = 1; // amount food adds
	   food = 0; // food on the board?
	   playing = 1; // are we playing the game?
	   players = 2; // number of players

	   memset( &movement[0][0], 0, sizeof(char)*SNAKES-1*BUF );
	   memset( &ahead[0], 0, sizeof(char)*SNAKES-1 );

	   memset( level.pce, 0,BSIZEX*BSIZEY );
	   memset( level.typ, 0, BSIZEX*BSIZEY );
	   memset( level.itm, 0, BSIZEX*BSIZEY );
	   memset( level.icn, 0, BSIZEX*BSIZEY );
	   memset( level.upd, 2, BSIZEX*BSIZEY ); // Why 2?! <shrug>

return 1;
}


char level_set(char h, char v)   // level reset
{

short c;

plr[1].startx = BSIZEX -2;
plr[1].starty = BSIZEY -2;
plr[1].dir = LEFT;


plr[0].startx = 1;
plr[0].starty = 1;
plr[0].dir = RIGHT;

for (c = players-1; c >= 0; c--) { // set up snakes..(temp)
 plr[c].siz = 0;
 plr[c].add = 3;
}


for (c = players-1; c >= 0; c--) {
 plr[c].bitx[ 0 ] = plr[c].startx;
 plr[c].bity[ 0 ] = plr[c].starty;
 level.itm[ plr[c].bity[ 0 ]*BSIZEX + plr[c].bitx[ 0 ] ] = c+1; // add start head
 level.icn[ plr[c].bity[ 0 ]*BSIZEX + plr[c].bitx[ 0 ] ] = // add start head icn
	plr[c].icn + (1<<(3-plr[c].dir)^15);
 plr[c].siz = 0;
 plr[c].add--;
}
//newlevel=1;

// 0=none 1=holes 2=full

if (h==1) {
for (c=2; c < BSIZEX-2; c++) { // horizontal wall
	  level.itm[c] = 4;
	  level.icn[c] = 4;

	  level.itm[c + (BSIZEY-1)*BSIZEX] = 4;
	  level.icn[c + (BSIZEY-1)*BSIZEX] = 4;
}
}

if (h==2) {
for (c=0; c < BSIZEX; c++) { // horizontal wall
	  level.itm[c] = 4;
	  level.icn[c] = 4;

	  level.itm[c + (BSIZEY-1)*BSIZEX] = 4;
	  level.icn[c + (BSIZEY-1)*BSIZEX] = 4;
}
}

if (v==1) {
for (c=2; c < BSIZEY-2; c++) { // verticle wall

   level.itm[c*BSIZEX] = 4;
   level.icn[c*BSIZEX] = 4;

   level.itm[c*BSIZEX +BSIZEX-1] = 4;
   level.icn[c*BSIZEX +BSIZEX-1] = 4;

}
}

if (v==2) {
for (c=0; c < BSIZEY; c++) { // verticle wall

   level.itm[c*BSIZEX] = 4;
   level.icn[c*BSIZEX] = 4;

   level.itm[c*BSIZEX +BSIZEX-1] = 4;
   level.icn[c*BSIZEX +BSIZEX-1] = 4;

}
}

return 1;
}

// (Cratylus:Messages): the ophidians were serpent worshippers ..
// the serpent was the symbol of wisdom etc etc .. the aphidians
// worshipped aphids .. <laugh>

