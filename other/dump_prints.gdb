# this script prints the locations of calls to graphicsclass::print
# and its arguments, it can then be culled with cull_redundant_prints.py
# script to see a history of how string colours change between arrow button presses

set logging file prints_info.txt
set logging redirect on
set logging overwrite on
set logging enabled on

set $btn_hit_counter = 0

b *0x469e20

commands $bpnum
silent
printf "printing string %s with colour (%x, %x, %x), retaddr is %p\n", *(char**)$rcx, $r8d, $r9d, *(long*)($rsp+8), *(long*)$rsp
c
end

b *0x46e25e

commands $bpnum
silent
printf "hit left or right, counter: %d\n", $btn_hit_counter
set $btn_hit_counter = $btn_hit_counter + 1
c
end

r
