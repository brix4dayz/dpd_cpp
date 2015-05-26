#! /bin/tcsh
#BSUB -q yingling
#BSUB -n 4
#BSUB -W 300:00
#BSUB -J 90_90
#BSUB -R "em64t span[ptile=2]"
#BSUB -o lammps.%J.out 
#BSUB -e lammps.%J.err 


setenv LD_LIBRARY_PATH /usr/lib/gcc/x86_64-redhat-linux/4.1.1:$LD_LIBRARY_PATH
setenv LD_RUN_PATH /usr/lib/gcc/x86_64-redhat-linux/4.1.1
source /usr/local/apps/mpich3/pgi_mpich3_hydra-134.csh
source /usr/local/apps/lammps/pgi64-hydra/lammps5Mar13.csh
set path = ($path /usr/local/apps/lammps/pgi64-hydra/lammps-5Mar13/src)
echo $PATH 

set PGI_LAMPS = "/usr/local/apps/lammps/pgi64-hydra/lammps-5Mar13/src/lmp_pgi"
unsetenv MPICH_NO_LOCAL
# run the matlab script



#mpiexec_hydra /usr/local/apps/lammps/pgi64-hydra/lammps-5Mar13/src/lmp_pgi < in.dpd > output.txt

alias acalc '    awk "BEGIN{print \!* }"'
set length = 98 #4 + 30  + 4
set volFrac = .1
set ttlBeads = "3*(36**3)"
set numChains = `acalc int(($volFrac * $ttlBeads / $length) + .5)`
set runnumber = 6 

foreach poly_number ($numChains) #num chains
  foreach poly_length  ($length) #4 + 4  + 4



cat<<eof>gendata2.m
clc, clear
%put 16 chains in a box 15*15*15
%total bead number is 3*15*15*15=10125(density is 3)
%diblock copolymer configuration:  o-o-o-o-o-o-o-o-o-O-O-O-O-O-O-O-O-O
%                                    9 hydrophobic     9 hydrophilic
%polymer bead number is 60*18=1080
%water bead number is 10125-1080 = 9045 (place water beads in x,y,z: 21*21*20=8820)
%r: coordinates for all atoms in polymers
%atoms: all information for atoms in polymers. It can be directly used  in newdata.dat file as input data for lammps
%atoms format:
% atom_index  atom_type  x  y  z  mol_index
%bonds: all bond information, can be directly used in newdata.dat file as input data for lammps
%bonds format:
% bond_index  bond_type atom_index1 atom_index2
%flows: all information for water atoms, can be directly used in newdata.dat file as input data for lammps

polymer_n=$poly_number;  %number of chains
polymer_len=$poly_length; %chain length
monomer_n=polymer_n*polymer_len;  %number of monomers polymer_n*polymer_len
hydrophobic_len=4;
hydrophillic_len = polymer_len  - 2*hydrophobic_len;


factor=0.1; %artificial bond length
ntype_atom=3;
ntype_bond=2;
lx=36;        ly=36;          lz=36; %when i change box size, ill have to change flow_n
box_x=lx;   box_y=ly;   box_z=lz;
flow_n=lx*ly*lz*3-monomer_n; % number of water beads 
polymer_sid=2; %distance between chains
r=zeros(monomer_n,3); %what is this





for index=0:1:polymer_n-1
    coordinates = rand(1,3);
    coordinatex = coordinates(1) * 34 + 1; %change this as well 
    coordinatey = coordinates(2) * 34 +1;
    coordinatez = coordinates(3) * 34 + 1; %starting position between 1 and 34, 
                                        %one away from edge of periodic
                                        %box.
  factor;
  i;
[movecoordinates] = random_polymerchain(factor,polymer_len, coordinatex, coordinatey, coordinatez, lx, ly, lz,1);
for j=1:1:polymer_len
    r(index * polymer_len + j , 1) = coordinatex  + movecoordinates(1)*j;
    r(index * polymer_len + j,2) = coordinatey + movecoordinates(2)*j;
    r(index * polymer_len + j,3) = coordinatez + movecoordinates(3)*j;
    index * polymer_len + j;
end;
end;

atoms=zeros(monomer_n,6); %for atoms in polymer chains

flows=zeros(flow_n,6); % for atoms in water
for i=1:1:monomer_n
    atoms(i,1)=i;
    atoms(i,6)=fix((i-1)/polymer_len)+1;
    if mod((i-1),polymer_len) <hydrophobic_len | mod((i-1), polymer_len) >= hydrophobic_len+hydrophillic_len
        atoms(i,2)=1;
    else
        atoms(i,2)=2;      %if the first 9 beads in a chain is type 1: hydrophobic
                           %the last 9 beads in a chain is type 2: hydrophilic
    end
    atoms(i,3)=r(i,1);
    atoms(i,4)=r(i,2);
    atoms(i,5)=r(i,3);
end
n_bond=(polymer_len-1)*polymer_n;
bonds=zeros(n_bond,4);   % for bond information
for i=1:polymer_n
    for j=1:polymer_len-1
        k=(i-1)*(polymer_len-1)+j;
		bonds(k,1)=k;
        if j<hydrophobic_len | j > hydrophobic_len+hydrophillic_len
		    bonds(k,2)=1;
		else
    		bonds(k,2)=2;
        end
        bonds(k,3)=(i-1)*polymer_len+j;
        bonds(k,4)=(i-1)*polymer_len+j+1;
        
    end
end
%% place water molecules in cubic
nx=round(flow_n^(1/3))+1; ny=nx; nz=nx; %place water beads in x,y,z: (3*36*36*36-1200)^(1/3)=51.7
flow_temp=zeros(nx*ny*nz,3);
flow_sid=0.6; %what is this?
k=1;
for i=0:1:nx-1
   for j=0:1:ny-1
      for l=0:1:nz-1
           flow_temp(k,1)=(i+0.5)*flow_sid;
           flow_temp(k,2)=(j+0.5)*flow_sid;
           flow_temp(k,3)=(l+0.5)*flow_sid;
           k=k+1;
	       if (k == flow_n)
                break
           end
      end
    if (k == flow_n)
        break
    end
   end
if (k == flow_n)
    break
end
end
      
for i=1:1:flow_n
    flows(i,1)=i+monomer_n;               
    flows(i,6)=fix((monomer_n-1)/polymer_len)+1+i;
    flows(i,2)=ntype_atom;
    flows(i,3)=flow_temp(i,1);
    flows(i,4)=flow_temp(i,2);
    flows(i,5)=flow_temp(i,3);
end

 %% write data file
fid_data = fopen('newdata.dat','w');

fprintf(fid_data,'LAMMPS Description\n');
fprintf(fid_data,'\n');
fprintf(fid_data,'%d atoms\n',monomer_n+flow_n);
fprintf(fid_data,'%d bonds\n',n_bond);

fprintf(fid_data,'\n');
fprintf(fid_data,'%d atom types\n',ntype_atom);
fprintf(fid_data,'%d bond types\n',ntype_bond);

fprintf(fid_data,'\n');
fprintf(fid_data,'%e %e xlo xhi\n',[0, lx]);
fprintf(fid_data,'%e %e ylo yhi\n',[0, ly]);
fprintf(fid_data,'%e %e zlo zhi\n',[0, lz]);
fprintf(fid_data,'\n');

fprintf(fid_data,'Atoms\n');
fprintf(fid_data,'\n');
for i=1: 1: monomer_n
    fprintf(fid_data,'%d %d %e %e %e %d\n',atoms(i,:));
end
for i=1: 1: flow_n
    fprintf(fid_data,'%d %d %e %e %e %d\n',flows(i,:));
end
fprintf(fid_data,'\n');

fprintf(fid_data,'Bonds\n');
fprintf(fid_data,'\n');
for i=1: 1: n_bond
    fprintf(fid_data,'%d %d %d %d\n',bonds(i,:));
end

fclose(fid_data);       


eof

if ($runnumber == 1) then
sleep 1

/usr/local/apps/matlab2009b/em64t/bin/matlab -nojvm -nodisplay -nosplash < gendata2.m > matlablog.dat 

sleep 5
mpiexec.hydra $PGI_LAMPS < dpd.in > $runnumber.no.$poly_number.len.$poly_length.txt

else
#for restart
sed "s/XXXX/$runnumber/g" template.in > restart_dpd.in

mpiexec.hydra $PGI_LAMPS < restart_dpd.in > $runnumber.no.$poly_number.len.$poly_length.txt
endif

   end
end
