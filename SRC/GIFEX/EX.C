
#include    <stdio.h>
#include    <malloc.h>

//extern showgif(int x1, int y1, int x2, int y2,void far *ptr,int setpal);

void showgif(int x1, int y1, int x2, int y2, char far *ptr,int setpal) {
		FILE *f=fopen("d:\snakey\dormgrab.gif","rb");
		char *buf=malloc(10000);      // ***** <=- erm :)

		fread(buf,1,10000,f);

		asm mov ax,0x13
		asm int 0x10

		showgif(0,0,319,199,buf,2);

erm:
        asm in  al,0x60
        asm cmp al,1
        asm jne erm

        asm mov ax,0x03
        asm int 0x10

        free(buf);
}
