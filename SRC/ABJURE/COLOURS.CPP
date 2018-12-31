// Done around July 1995

#include <MEM.H>
#include <iostream.H>
#include <conio.H>
#include "colours.hpp"

#define uc unsigned char
#define us unsigned short

   ColourPalette::ColourPalette() { // Make palette ready for use, 0 = black 1 = white
		clearPalette();
   }

   void ColourPalette::clearPalette() { // Clear palette
	 fill_ = 0;
	 gauge_ = 0;

	 memset( table_,  32, 768); // clear table[]
	 memset( used_,   0,  256*sizeof(us)); // clear used[]
	 memset( nsc_,    0,  256); // clear nsc[]

	 addCol(0,0,63, 0,254); // green for white?
	 addCol(0,0,0, 0,255); // black background
   }
   uc ColourPalette::addCol(uc r, uc g, uc b, uc nsc ) {
				 // add a 0=static/1=nonstatic colour to palette,
				 // returns where it is in the palette (next avalible space)

	us i;

	for (i = 0; i<256; i++) { // search for already used same colour
		if (table_[i][0] == r &&
			table_[i][1] == g &&
			table_[i][2] == b &&
			nsc_[i] == 0 &&
			used_[i] > 0) {
				used_[i]++;
				return i;
			}
	}

	// new colour!

		if (gauge_ < 256) { // find a place...
			while (used_[fill_] > 0) fill_++;
		}
		else {
			//throw("Palette full!");
			cout << "Palette Full!\n";
		}

		newCol(r, g, b, nsc, fill_ );

		return fill_;

   }

   uc ColourPalette::addCol(uc r, uc g, uc b, uc nsc, uc place ) {

		if (used_[place] > 0) { // Error! Space in use!
			cout << "ColourPalette::addCol(uc r, uc g, uc b, uc nsc, uc place ): place in use\n";
			return place -1;
		}
		newCol(r,g,b,nsc,place);
		return place;
   }

   uc ColourPalette::addCol( uc place ) { // add one to colour use
		if (used_[place] == 0) { // Error! Space not in use!
			return place -1;
		}
		used_[place]++;
		return place;
   }

   void ColourPalette::newCol(uc r, uc g, uc b, uc nsc, uc place ) { // for NEW colours. No checking.

		table_[place][0] = r;
		table_[place][1] = b;
		table_[place][2] = g;
		nsc_[place] = nsc;
		used_[place] = 1;

		gauge_++;
		//return 0;

   }

   void ColourPalette::changeNSC(uc col, uc r, uc g, uc b) { // change a nonstatic colour
	 // No checking if really is a NSC...

	 table_[col][0] = r;
	 table_[col][1] = b;
	 table_[col][2] = g;
   }

   void ColourPalette::removeCol(uc col) {
	 if (used_[col] > 0) used_[col]--;
	 //else throw("Tried to remove dead colour!");
	 if (used_[col] == 0 && fill_ > col) fill_ = col;
   }


   void ColourPalette::use() { // send whole palette to video card
	uc *t = &table_[0][0];
	asm{
		mov	dx, 0x3c8	// Select Starting Color Register..
		mov al, 1
		out dx, al

		inc dx			// Select Data Loading register..

		mov cx, 256
		}
	loop:

		_AL = *t;
	asm out dx, al
	t++;

		_AL = *t;
	asm out dx, al
	t++;

		_AL = *t;
	asm out dx, al
	t++;


	asm dec cx
	asm jnz loop;

	asm sti



   }



   void ColourPalette::use(uc col) { // send single colour to video card
	uc *t = &table_[col][0];
	us temp = col+1;
	asm{

		mov dx, 0x3C6 // palette mask
		mov al, 0xFF
		out dx, al

		mov	dx, 0x3c8 // Select Starting Color Register..
		mov ax, temp
		out dx, al

		//inc dx		  // Select Data Loading register..
		mov dx, 0x3c9 // palette data register

		}
//	loop:

		_AL = *t;
	asm out dx, al
	t++;

		_AL = *t;
	asm out dx, al
	t++;

		_AL = *t;
	asm out dx, al
	t++;


//	asm dec cx
//	asm jnz loop;

//	asm sti



   }


