(
"sporth/contrenot.sp" load
_freq get 24 + mtof 0.3 
_fsr get 0.1 0.5 scale 0.1 port
square 
_freq get 23.8 + mtof 0.3 
_fsr get 0.3 0.5 scale 0.1 port
square +
2000 butlp 
_bow get * 
dup
)
