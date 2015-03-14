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
	make testGenerateData
	make testDLLInPython

testBead:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -DTESTING -o bead vect.o bead.cpp
	./bead

testBeadFile:
	g++ -Wall -std=c++11 -c vect.cpp
	echo "------------ BEAD ---------" >> mac_results
	g++ -Wall -std=c++11 -DTESTING -o bead vect.o bead.cpp
	./bead >> mac_results
	echo "" >> mac_results

testBlock:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -DTESTING -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock

testBlockFile:
	echo "---------- BLOCK -------------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -DTESTING -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock >> mac_results
	echo "" >> mac_results


testTriblock:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -DTESTING -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock


testTriblockFile:
	echo "---------- TRIBLOCK -----------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -DTESTING -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock >> mac_results
	echo "" >> mac_results

testBin:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -DTESTING -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest


testBinFile:
	echo "------------ BIN ------------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -DTESTING -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest >> mac_results
	echo "" >> mac_results

testCore:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -DTESTING -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest

testCoreFile:
	echo "------------ CORE ---------------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -DTESTING -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest >> mac_results
	echo "" >> mac_results

testMicelle:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -c hydrophobicCore.cpp
	g++ -Wall -std=c++11 -DTESTING -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest


testMicelleFile:
	echo "------------ MICELLE ---------------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -c hydrophobicCore.cpp
	g++ -Wall -std=c++11 -DTESTING -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest >> mac_results
	echo ""	>> mac_results


testFrameFile:
	echo "------------------ FRAME -------------------" >> mac_results
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -c hydrophobicCore.cpp
	g++ -Wall -std=c++11 -c micelle.cpp
	g++ -Wall -std=c++11 -DTESTING -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest >> mac_results
	echo "" >> mac_results

testFrame:
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -c hydrophobicCore.cpp
	g++ -Wall -std=c++11 -c micelle.cpp
	g++ -Wall -std=c++11 -DTESTING -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest

testGenerateData:
	rm -rf *.o
	g++ -Wall -std=c++11 -c vect.cpp
	g++ -Wall -std=c++11 -c bead.cpp
	g++ -Wall -std=c++11 -c polymerBlock.cpp
	g++ -Wall -std=c++11 -c pecTriblock.cpp
	g++ -Wall -std=c++11 -c bin.cpp
	g++ -Wall -std=c++11 -DTESTING -o dataTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o dpdPolymerData.cpp
	mv newdata formerdata
	mv newdataTest.dat formerdataTest.dat
	./dataTest > newdata
	diff formerdata newdata
	diff newdataTest.dat formerdataTest.dat

testDLLInPython:
	rm -rf *.o *.dylib
	g++ -Wall -std=c++11 -DPYTHON_LIB -DTESTING -c -fPIC vect.cpp -o vect.o && g++ -Wall -std=c++11 -shared -Wl -o libvect.dylib vect.o
	mv current_libvect_test former_libvect_test
	python2.7 test_libvect.py > current_libvect_test
	diff current_libvect_test former_libvect_test
