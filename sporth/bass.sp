(
"sporth/contrenot.sp" load

_freq get 24 + mtof 0.01 port 0.5 saw 
_freq get 36.1 + mtof 0.01 port 0.3 saw 
_freq get 35.9 + mtof 0.01 port 0.3 saw 
+ + 

3000 0.1 moogladder

_bow get *

dup
)
