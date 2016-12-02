(
"sporth/contrenot.sp" load
_fsr get 0.5 0 thresh 
_freq get 48 + mtof
0.5 200 pluck 

_fsr get 0.5 0 thresh 
_freq get 52 + mtof
0.5 200 pluck +

dup 0.9 0.6 delay 1000 butlp -5 ampdb * _bow get * +

dup dup 10 10 8000 zrev drop 0.3 * _bow get * +

dup
)
