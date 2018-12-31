Infi: go through it and have a go at it, you;ll find you can select ANY colour
from the VGA pallette and paste it, although its only set for 80x25x16 but you
should nbe able to change that to 320x200x256 (13h)

Infi: in palette, define as db   r, g, b  and you should be able to use a
single colour or all 256 :) hope that helps

Infi: palette:
Infi:         db      15*3 dup (?)
Infi: change to:
Infi: Palette: db     r,g,b
Infi: 15*3 is just randoming colours ie: first 16 just change it


        ideal
        model   tiny
        p386n
segment cseg    para public 'code'
        assume  cs:cseg,ds:cseg,es:cseg,ss:cseg
        org     100h
start:
        mov     dx,03c8h
        mov     al,1
        out     dx,al
        inc     dx
        mov     cx,15*3
        mov     di,offset palette
        rep     insb
        sub     ax,ax
        mov     es,ax
        mov     eax,[es:01ch*4]
        mov     [oldint],eax
        cli
        mov     [word es:01ch*4],offset myint
        mov     ax,cs
        mov     [es:01ch*4+2],ax
        sti
        mov     ax,3100h
        lea     dx,[endofprog+100h]
        shr     dx,4
        int     21h
proc    myint
        pushf
        pusha
        push    ds
        mov     ax,cs
        mov     ds,ax
        mov     bl,[fc]
        dec     [fc]
        test    bl,01000000b
        jz      @j1
        not     bl
@j1:
        and     bl,00111111b
        mov     dx,03c8h
        mov     al,1
        out     dx,al
        inc     dx
        mov     si,offset palette
        mov     cx,15*3
@l1:
        lodsb
        mov     ah,0
        mul     bl
        shr     ax,6
        out     dx,al
        loop    @l1
        pop     ds
        popa
        popf
        jmp     [dword cs:oldint]
endp    myint
oldint  dd      ?
palette:
        db      15*3 dup (?)
fc      db      0
endofprog:
ends    cseg
end     start

