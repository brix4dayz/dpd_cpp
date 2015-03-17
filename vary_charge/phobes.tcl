color Display Background white
package require pbctools
pbc set {36 36 36} -all
pbc box

mol delrep 0 top
mol representation VDW 0.2200000 152.000000
mol color ColorID 32
mol selection {name H}
mol material BrushedMetal
mol addrep top
mol selupdate 0 top 0
mol colupdate 0 top 0
mol scaleminmax top 0 0.000000 0.000000
mol smoothrep top 0 0
mol drawframes top 0 {now}
mol clipplane center 0 0 top {0.0 0.0 0.0}
mol clipplane color  0 0 top {0.5 0.5 0.5 }
mol clipplane normal 0 0 top {0.0 0.0 1.0}
mol clipplane status 0 0 top {0}
mol clipplane center 1 0 top {0.0 0.0 0.0}
mol clipplane color  1 0 top {0.5 0.5 0.5 }
mol clipplane normal 1 0 top {0.0 0.0 1.0}
mol clipplane status 1 0 top {0}
mol clipplane center 2 0 top {0.0 0.0 0.0}
mol clipplane color  2 0 top {0.5 0.5 0.5 }
mol clipplane normal 2 0 top {0.0 0.0 1.0}
mol clipplane status 2 0 top {0}
mol clipplane center 3 0 top {0.0 0.0 0.0}
mol clipplane color  3 0 top {0.5 0.5 0.5 }
mol clipplane normal 3 0 top {0.0 0.0 1.0}
mol clipplane status 3 0 top {0}
mol clipplane center 4 0 top {0.0 0.0 0.0}
mol clipplane color  4 0 top {0.5 0.5 0.5 }
mol clipplane normal 4 0 top {0.0 0.0 1.0}
mol clipplane status 4 0 top {0}
mol clipplane center 5 0 top {0.0 0.0 0.0}
mol clipplane color  5 0 top {0.5 0.5 0.5 }
mol clipplane normal 5 0 top {0.0 0.0 1.0}
mol clipplane status 5 0 top {0}

label textsize 1.0
display ambientocclusion on
display shadows on
display resize 2400 2400
display resetview
display projection Orthographic 
display depthcue off
axes location off
rotate x by 90
display projection Perspective
#rotate y by 5
scale by .85


render snapshot init.tga

#quit