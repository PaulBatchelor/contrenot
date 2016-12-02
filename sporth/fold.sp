(
"sporth/contrenot.sp" load

_saw 4096 "0 1 4095 -1" gen_line

_freq get round 24 + mtof 0.1 port 0 phasor 
10 inv 1 sine 1.4 2 biscale *
1 0 1 _saw tabread 

_freq get round 36.1 + mtof 0.1 port 0 phasor 
10 inv 1 sine 1 2 biscale *
1 0 1 _saw tabread +

1000 0.1 3 wpkorg35

-10 ampdb * 
_freq get round 36 + mtof 0.1 port 0.3 tri + 
_bowenv get * 

_dry var _dry set

_dry get 1000 buthp 
dup 0.7 0.6 delay -6 ampdb * + 
dup 0.91 8000 revsc _dry get + swap _dry get + 
)
