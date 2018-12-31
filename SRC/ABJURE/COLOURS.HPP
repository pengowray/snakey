//#include <MEM.H>

#define uc unsigned char
#define us unsigned short

#define ID unsigned short
#define LOC unsigned short
#define ICN unsigned char


// struct _Colours {  uc r;  uc g;  uc b;  };
// #define Colours struct _Colours


class ColourPalette { // manages the palette
  public:
   ColourPalette(); // Make palette ready for use, 0 = black 1 = white
   uc addCol(uc r, uc g, uc b, uc nsc);
   uc addCol(uc r, uc g, uc b, uc nsc, uc place );
   uc addCol( uc place ); // add one to colour use
   void newCol(uc r, uc g, uc b, uc nsc, uc place ); // for NEW colours. No checking.

   void clearPalette();
   void use(); // send whole palette to video card
   void use(uc col); // send single colour to video card
   void changeNSC(uc col, uc r, uc g, uc b); // change a nonstatic colour
   void removeCol(uc col);

  private:
// r=red b=green g=blue <sulk>
   uc table_[256][3]; //768]; // RGBRGBRGB.. 0-63
   us used_[256]; // times each colour's used (for deconstructing)
   uc nsc_[256]; // is the colour non-static? (to be cycled)
   us gauge_; // number of colours used (incase we fill them all up)
   uc fill_; // colour we're up to in filling up palette

};


// The Queen's weight == 51kg at 84 and has been fairly steady
