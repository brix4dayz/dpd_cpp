
for {set i 0} { $i < 4 } {incr i} {
  mol load xyz core$i.xyz
}

pbc set { 36 36 36 } -all
pbc box