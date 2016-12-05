(
"sporth/contrenot.sp" load

_clk var
_fsr get 0.1 0 thresh  120 4 clock _clk set

_seq "0 7 12" gen_vals

_clk get 0.01 0.01 0.1 tenvx 

_freq get 48 +
_clk get 0 _seq tseq + 
mtof 0.3 tri 

_freq get 48 0.2 _fsr get * +  +
_clk get 0 _seq tseq + 
mtof 0.3 tri +

*

_bowenv get * 

dup
)
