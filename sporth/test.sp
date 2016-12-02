_contrenot "./serial.so" fl
_sp _fsr _rot _vel _contrenot fe 

_sp get 0.01 port 60 + mtof _fsr get 0.1 gt samphold 0.5 1 1 
_fsr get 0.1 3 scale 0.005 port

fm

_rot get 0.5 2 thresh 0.1 tgate 
_vel get 20 lt * 
0.008 0.01 1.0 0.1 adsr  0.05 port *

 2 _vel get 0.01 port dup 40 lt samphold
#"val" print 
0.0001 + / 0.0 1.5 limit 0.009 port *

#"val" print drop

#drop 

#_rot get 0.5 2 thresh 0.001 0.001 0.001 tenvx 1000 0.5 sine *


dup dup 0.9 10000 revsc drop -20 ampdb * + 

_fsr get "val" print drop

_contrenot fc

