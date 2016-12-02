(
"sporth/contrenot.sp" load

_freq get
60 + mtof 
0.5 1 1 
_fsr get 0.1 3 scale 0.005 port
fm

_bow get *

dup dup 0.9 10000 revsc drop -20 ampdb * + 

dup
)
