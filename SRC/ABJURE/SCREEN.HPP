#define uc unsigned char
#define us unsigned short

void init( void ); // go into VGA
void deinit( void ); // out of VGA
void waitv(void); // wait for verticle retrace
void pset(us x, uc y, uc col, char far *p);
//inline void pset(us x, uc y, uc col, char far *p);
