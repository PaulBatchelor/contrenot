_contrenot "./serial.so" fl
_sp _count _rot _contrenot fe 

_sp get 0.01 port 60 + mtof 0.5 1 1 1 fm

_rot get 0.5 2 thresh 0.05 tgate 0.008 0.01 1.0 0.1 adsr *

2 _count get 0.0001 + / 0.0 1.5 limit 0.009 port *

#"val" print drop

#drop 

#_rot get 0.5 2 thresh 0.001 0.001 0.001 tenvx 1000 0.5 sine *


dup dup 0.9 10000 revsc drop -20 ampdb * + 

_contrenot fc


