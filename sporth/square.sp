(
"sporth/contrenot.sp" load
_freq get 60 + 0.005 port mtof 0.3 
_fsr get 0.1 0.5 scale 0.1 port
square 2000 butlp 
_bow get * 
dup
)
