(
"sporth/contrenot.sp" load

_index var
_fsr get 1 3 scale 0.1 port _index set

_freq get 59.9 + mtof 0.5 1 1 _index get fm
_freq get 60 + mtof 0.5 1 1.01 _index get fm +
_freq get 60.1 + mtof 0.5 1 0.991 _index get fm +
-6 ampdb *

_freq get 66.9 + mtof 0.5 1 1 _index get fm
_freq get 67 + mtof 0.5 1 1.004 _index get fm +
_freq get 67.01 + mtof 0.5 1 0.991 _index get fm +
-6 ampdb * +

-6 ampdb * 
_bow get * 

dup 0.6 1.1 delay -10 ampdb * 1000 butlp + 

dup dup 0.93 5000 revsc drop -10 ampdb * dcblk + 

dup
)
