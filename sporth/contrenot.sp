_contrenot "./serial.so" fl
_sp _fsr _rot _vel _contrenot fe 
_contrenot fc

_bowenv var
_bowvel var 
_bow var
_freq var

_rot get 0.5 2 thresh 0.1 tgate 
_vel get 20 lt * 
0.008 0.01 1.0 0.1 adsr  0.05 port _bowenv set

2 _vel get 0.01 port dup 40 lt samphold
0.0001 + / 0.0 1.5 limit 0.009 port _bowvel set

_bowenv get _bowvel get * _bow set

_sp get 0.01 port 
_fsr get 0.005 gt samphold _freq set

