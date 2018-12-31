// File IO for graphics... and database thing?

#define uc unsigned char
#define us unsigned short


const char header[] = {"Abjure Graphics File\r\n\0"};
#define HEADERSIZE 23
// Main Header:
//                  "Abjure Graphics File\r\n\0" (HEADERSIZE chars)

// Main table:
//
// 8	Area: Xxxxxxxx. (Main, Tim's, Castle, General)
// 2	Number of items
// 4	Actual data starts at (position)
// 14x? List of: 4 where in file after start position
//               4 Type, XxXx. (Walls, animation, still..), case sensitive
//               2 ID of items (duplicated data)
//               1 Ver

// Each item:
//
/*
  // 4	Type:
  // 2	ID number: nnnn (connected with above)
  // 8	File name: XXXXXXXX. (FISHNOSE, HELLO   , ... )
  // 1	Ver: nn (v206)
*/
// ?	Item Name + Creator name + info: Orcish Troll\nSasha Simon\nA real nice troll.\0
// 3    Extension: Xxxx. (PNG, MPG, GIF, JPG, Tif, ...)
// 4	Data size of below (43,432 bytes)
// ?    Actual Data (gif or whatever)

// Resources not in a graphics file will be stored as:
// <area> . <closest avalible number to 0 but not 0>

struct Area {
  char name[9];
};

struct Filename {
  char name[9];
};

#define Ver unsigned short

getPic( Area area, Type type, ID id, Ver ver ) {

FILE file;
int c;
char tstr[35];

/// LOCATE IMAGE ///

// check if file is AREA file (ABJ) is already open...

// or open file needed...
if (file = openFile (Area area) == NULL) return -1; // throw "No file"
// get HEADERSIZE characters
fread(tstr, 1, HEADERSIZE, file);
strcmp(tstr, header);
// look for type/id in file...

  // get number of entries..

  // search..
for (c=0; c < total; c++) {

}

// If no image then start DLing / whatever

/// INPUT IMAGE AND PALETTE

}

FILE openFile( Area area ) {
  char filename[20];
  FILE file;
  strcpy (filename, area.name);
  strcat (filename, ".ABJ");
  file = fopen (filename, "r+");

  return file;
}