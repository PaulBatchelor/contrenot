(
"sporth/contrenot.sp" load

_buf 5 sr * zeros 

_fsr get 8 35 scale metro
0.001 0.001 0.001 tenvx _freq get 2 * 60 + mtof 0.5 sine *

dup tick _buf tblrec

0 1 
0.1 3 2 
randh randi 1 0 0 _buf tabread -4 ampdb * 1000 butlp 
12 1024 512 pshift
+ 

_bow get * 
dup
)
