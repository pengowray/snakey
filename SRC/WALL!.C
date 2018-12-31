float wall (short YY, short XX, unsigned char fcol, unsigned char col, char far *p)
{

//#define XX 200
//#define YY 320

short c,i,j;
float side;
long double corna,cornb,x,y,z;

randomize();

do {
  side = (float)random(80)/8 +1;//pow( ((random(7)/2+1)), 2 );
} while ( (float)sqrt(side) == (int)sqrt(side) );

corna= -XX/2 * side;
cornb= -YY/2 * side;

if (col < 2) col=2;


////// Tada

for (i = 0; i < XX; i++) {
  for (j = 0; j < YY; j++) {

	x = corna + i * side;
	y = cornb + j * side;

	c = x*x + y*y;

	*p++ = (c % col) + fcol;
  }
}

return side;



}

