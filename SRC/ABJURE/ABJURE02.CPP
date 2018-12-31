// c++ version of snakey...
// Started last night.. 9th July

#include <ALLOC.H>
#include <MEM.H>
#include <CONIO.H>
#include <DOS.H>
#include <IOSTREAM.H>
#include "colours.hpp"
#include "screen.hpp"

#define uc unsigned char
#define us unsigned short

#define ID unsigned short
#define LOC unsigned short
#define ICN unsigned char

class Board {

 public:
  Board(us BSIZEX, us BSIZEY) {
   X_ = BSIZEX; Y_ = BSIZEY;
   stationary_ = (ICN *) malloc(sizeof(ICN)*X_*Y_);
   moving_     = (ICN *) malloc(sizeof(ICN)*X_*Y_);
   id_         = (ID  *) malloc(sizeof(ID)*X_*Y_);
   hwall_      = (LOC *) malloc(sizeof(LOC)*(X_+1)*(Y_+1));
   vwall_      = (LOC *) malloc(sizeof(LOC)*(X_+1)*(Y_+1));
   clear();
  }

 ~Board() {
   free(stationary_);
   free(moving_);
   free(id_);
   free(hwall_);
   free(vwall_);
 }

 void addPiece(us x, us y, us id, us icon) {
   id_[y*X_ + x] = id;
   moving_[y*X_+x] = icon;
 }

 void killPiece(us x, us y) {
   id_[y*X_ + x] = 0;
   moving_[y*X_+x] = 0;
 }

 void clear() {
	memset( stationary_, 0, X_*Y_ );
	memset( moving_,     0, X_*Y_ );
	memset( id_,         0, X_*Y_ );
	memset( hwall_,      0, (X_+1)*(Y_+1) );
	memset( vwall_,      0, (X_+1)*(Y_+1) );
 }

 private:
  us X_, Y_; // size of board
  ICN *stationary_;
  ICN *moving_;
  ID  *id_;
  LOC *hwall_;
  LOC *vwall_;

};



class IconPalette { // manages icons

  public:

   void drawIcon(ICN icon, LOC x, LOC y); // draw an icon on board
   us addIcon(uc *p); // add an icon, returns where it is in the palette
   void changeIcon(us icn, uc *p);
   void removeIcon(us icn);

  protected:

   uc *icon_; // pointer to: pointers to icons, (and owner's ID?)
   ICN fill_; // Number of icons
   us xsize_; // size of icon (all sizes the same in palette)
   us ysize_;
};

class Image { // for weird icons/images/graphics/fonts?

  public:
   us xsize_;
   us ysize_;
   uc *graphic_;
};

class ItemPalette {
   //ID addItem(class *item); // add an item, returns ID code
   //void changeItem(ID id, class *item);
   void removeItem(ID id);
 private:
   uc *item_; // pointer to: pointers to items, type, ID
   ID fill_; // Number of items
};

class Item { // ABC

public:

LOC x,y; // for 1x1 items
void beginMove();
void finishMove();
void nextMove();
void icon(); // How to draw icons... for fractal items and the like
void collision(); // ummmmmm.......

protected:

ICN icon_; // where in icon palette
uc type_; // type of item, eg snake, food etc...
ID id_; // ID of particular item
uc weight_; // weight of item, for working out collisions.
		// 0 can go through without collision, 1 can go through but still
		// colides, 2 edible, 3+ dangerous.
		//           OR
		// |0|1|2|3|4|5|6|7|8|9... bits
		// 0 - stop snake going through (a wire mesh)
		// 1 - stop creatures going through (a secret passage)
		// 2 - stop anything going through (a wall)
		// 3 - edible (ice cream cone, kabab, lasagna)
		// 4 - let pass but must go through collision thing (teleporter)
		// 5 - may do damage (fire)
		// 6 - gettable item (various religious artifacts, security cards)
		// 7 - may take you to a totally new place (stairs, ladder)
		// 8 - animated (waterfall)
		// 9 - allows collisions with certain items (locked door)
		// 10 - produces items (factory)
		// 11 - looks different from different angles (3D object)
		// 12 - Sound source (speaker)
		// 13 - icon doesn't reflect actual item (trap, invisible fish)
		// 14 - Container (bag, shelf)
		//                   ·─═ OR ═─·
		// |0|1|2|3|4|5|6|7| bits
		// 0 - Must check colision routine when coliding with anything
		// 1 - Must check colision routine when coliding with snake
		// 2 - Must check colision routine when coliding with food...
		// 4 - Etc...
		// A bit for every item type.

};

class Snake : Item {
 public:
  Snake(LOC startx, LOC starty, uc dir, us siz) {
	type_ = 2;
  }
  void lenAdd(us );
  void lenShrink(us );
  void lenMake(us );
 protected:
  LOC *bitx_; // snake position..  (x)
  LOC *bity_; // snake position..  (y)
  us arraysiz_; // size of snake's array
  us siz_; // size of snake
  short add_; // amount to add to current length of snake

  uc dir_; // direction. (0 - 3)
  LOC startx_; // starting position of snake
  LOC starty_;
  ICN icn_; // where is 1st icon of 20?  0-15 body+tail. 16-19 head
  char crash_; // crashed?

};

class Food : Item {

  Food() {
	//Icon( char whatever );
  }
  void nextMove( );
  void move( );
  void collision() {
	 //if (collider == snake) { remove food; FoodMaster(); }
  }
};


class Wall : Item {

};

class Badie : Item {

};

void main (void) {
char far *screen;
ColourPalette colpal;
ColourPalette title;

//colpal = new ColourPalette();

screen = (char far *) MK_FP( 0xA000, 0);

init();

*screen = 1;

for ( us i = 0; i < 257; i++ ) {
  for ( us ii = 0; ii < 200; ii++ ) {
	pset(i,ii,(uc)i,screen);
  }
}
getch();
colpal.use();
getch();

for ( i = 0; i < 64; i++ ) {
//  cout << (us)colpal.AddCol((uc)i,(uc)i,(uc)i,0) << " ";
	(us)colpal.addCol(i,0,i, 0);
	(us)colpal.addCol(0,i,i, 0);
	(us)colpal.addCol(i,i,0, 0);
	(us)colpal.addCol(0,i,0, 0);
	(us)colpal.addCol(i,0,0, 0);
//	(us)colpal.addCol(i,i,i, 0);

//  colpal.addCol((uc)i,(uc)i,(uc)i,0);
//  colpal.changeNSC(i,i,i,0);
}

//getch();

//init();


colpal.use();

char bgr=0, bgb=0, bgg=0, c=0, cc=0;
char up=+1, dn=-1, mx = 63;

while (!kbhit()){
  c++;
  cc++;
  if (cc==64) {
	cc=0;
	up *= -1;
	dn *= -1;
  }
			 // use(c)   use()
  bgr  = up; // nothing, blue
  bgg += up; // blue,    green
  bgb += dn; // green,   red

  //bgg=short(bgb+1)%short(bgr+1);
  colpal.changeNSC(c, bgb, bgr, bgg);
  colpal.changeNSC(255, bgb, bgr, bgg);
  //colpal.use();
  colpal.use(c);
  colpal.use(255);
  waitv();


}

getch();
deinit();


}
