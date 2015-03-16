
ifdef LINUX
STDFLAG=-std=gnu++11
CC=g++-4.8
else
STDFLAG=-std=c++11
CC=g++
endif

WARN=-Wall

results:
	rm -rf *.o
	mv mac_results old_mac_results
	make testBeadFile
	make testBlockFile
	make testTriblockFile
	make testBinFile
	make testCoreFile
	make testMicelleFile
	make testFrameFile
	rm -rf *.o
	diff mac_results old_mac_results
ifndef LINUX
	make testGenerateData
	make testDLLInPython
endif

testBead:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o bead vect.o bead.cpp
	./bead

testBeadFile:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	echo "------------ BEAD ---------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o bead vect.o bead.cpp
	./bead >> mac_results
	echo "" >> mac_results

testBlock:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock

testBlockFile:
	echo "---------- BLOCK -------------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock >> mac_results
	echo "" >> mac_results


testTriblock:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock


testTriblockFile:
	echo "---------- TRIBLOCK -----------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock >> mac_results
	echo "" >> mac_results

testBin:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest


testBinFile:
	echo "------------ BIN ------------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest >> mac_results
	echo "" >> mac_results

testCore:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest

testCoreFile:
	echo "------------ CORE ---------------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest >> mac_results
	echo "" >> mac_results

testMicelle:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest


testMicelleFile:
	echo "------------ MICELLE ---------------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest >> mac_results
	echo ""	>> mac_results


testFrameFile:
	echo "------------------ FRAME -------------------" >> mac_results
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest >> mac_results
	echo "" >> mac_results

testFrame:
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest

testGenerateData:
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -DTESTING -o dataTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o dpdPolymerData.cpp
	mv newdata formerdata
	mv newdataTest.dat formerdataTest.dat
	./dataTest > newdata
	diff formerdata newdata
	diff newdataTest.dat formerdataTest.dat

testDLLInPython:
	rm -rf *.o *.dylib
	$(CC) $(WARN) $(STDFLAG) -DPYTHON_LIB -DTESTING -c -fPIC vect.cpp -o vect.o && $(CC) $(WARN) $(STDFLAG) -shared -Wl -o libvect.dylib vect.o
	mv current_libvect_test former_libvect_test
	python2.7 dpdVect.py > current_libvect_test
	diff current_libvect_test former_libvect_test
