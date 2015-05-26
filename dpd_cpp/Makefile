
ifdef LINUX
STDFLAG=-std=gnu++11
ifdef HPC
CC=/usr/local/apps/gcc/473/bin/g++
else
CC=g++-4.8
endif
else
STDFLAG=-std=c++11
CC=g++
endif

WARN=-Wall
DEBUG=-g
TEST=-DTESTING
LIBDPD=-ldpd -L/gpfs_partners/yingling/backup/Fuss/dpd_cpp/dpd_cpp/lib/

results:
	rm -rf *.o *.dSYM
	mv mac_results old_mac_results
	make testBeadFile
	make testBlockFile
	make testTriblockFile
	make testBinFile
	make testCoreFile
	make testMicelleFile
	make testFrameFile
	rm -rf *.o *.dSYM
	diff mac_results old_mac_results
ifndef LINUX
	make testGenerateData
	make testDLLInPython
	rm -rf *.o *.dSYM
endif

testBead:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o bead vect.o bead.cpp
	./bead

testBeadFile:
	echo "------------ BEAD ---------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -o bead vect.o bead.cpp
	./bead >> mac_results
	echo "" >> mac_results

testBlock:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock

testBlockFile:
	echo "---------- BLOCK -------------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -o polymerBlock vect.o bead.o polymerBlock.cpp
	./polymerBlock >> mac_results
	echo "" >> mac_results


testTriblock:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock


testTriblockFile:
	echo "---------- TRIBLOCK -----------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o triblock vect.o bead.o polymerBlock.o pecTriblock.cpp
	./triblock >> mac_results
	echo "" >> mac_results

testBin:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest


testBinFile:
	echo "------------ BIN ------------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o binTest vect.o bead.o polymerBlock.o pecTriblock.o bin.cpp
	./binTest >> mac_results
	echo "" >> mac_results

testCore:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest

testCoreFile:
	echo "------------ CORE ---------------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o coreTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.cpp
	./coreTest >> mac_results
	echo "" >> mac_results

testMicelle:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest


testMicelleFile:
	echo "------------ MICELLE ---------------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o micelleTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.cpp
	./micelleTest >> mac_results
	echo ""	>> mac_results


testFrameFile:
	echo "------------------ FRAME -------------------" >> mac_results
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest >> mac_results
	echo "" >> mac_results

testFrame:
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o frameTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.cpp
	./frameTest

testGenerateData:
	rm -rf *.o
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG)  $(TEST) -o dataTest vect.o bead.o polymerBlock.o pecTriblock.o bin.o dpdPolymerData.cpp
ifndef LINUX
	mv newdata formerdata
	mv newdataTest.dat formerdataTest.dat
	./dataTest > newdata
	diff formerdata newdata
	diff newdataTest.dat formerdataTest.dat
endif

triblockBuilder:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c dpdPolymerData.cpp
	$(CC) $(WARN) $(STDFLAG) -o triblockBuilder vect.o bead.o polymerBlock.o pecTriblock.o bin.o dpdPolymerData.o chargeTriblockBuilder.cpp
else
	$(CC) $(WARN) $(STDFLAG) $(LIBDPD) -o triblockBuilder chargeTriblockBuilder.cpp
endif


testDLLInPython:
	rm -rf *.o *.dylib *.dSYM
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -DPYTHON_LIB $(TEST) -c -fPIC vect.cpp -o vect.o && $(CC) $(WARN) $(DEBUG) $(STDFLAG) $(TEST) -shared -Wl -o libvect.dylib vect.o
	mv current_libvect_test former_libvect_test
	python2.7 dpdVect.py > current_libvect_test
	diff current_libvect_test former_libvect_test

testTriblockProcessor:
	rm -rf *.o
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -c copolymerMicelleFrame.cpp
	$(CC) $(WARN) $(DEBUG) $(STDFLAG) -DUSING -o triblockProcessor dpdTrajectory.cpp copolymerMicelleFrame.o micelle.o hydrophobicCore.o bin.o pecTriblock.o polymerBlock.o bead.o vect.o

libdpd.so:
	rm -rf ./lib/*.o ./lib/libdpd.so
	$(CC) $(WARN) $(STDFLAG) -c -fPIC vect.cpp -o vect.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC bead.cpp -o bead.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC polymerBlock.cpp -o polymerBlock.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC pecTriblock.cpp -o pecTriblock.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC linkerPECTriblock.cpp -o linkerPECTriblock.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC bin.cpp -o bin.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC hydrophobicCore.cpp -o hydrophobicCore.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC micelle.cpp -o micelle.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC copolymerMicelleFrame.cpp -o copolymerMicelleFrame.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC dpdTrajectory.cpp -o dpdTrajectory.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC triblockTimeEvolution.cpp -o triblockTimeEvolution.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC dpdPolymerData.cpp -o dpdPolymerData.o
	$(CC) $(WARN) $(STDFLAG) -c -fPIC linkerTriblockData.cpp -o linkerTriblockData.o	
	$(CC) $(WARN) $(STDFLAG) -shared -Wl,-soname,libdpd.so -o libdpd.so vect.o bead.o polymerBlock.o pecTriblock.o bin.o hydrophobicCore.o micelle.o copolymerMicelleFrame.o dpdPolymerData.o dpdTrajectory.o triblockTimeEvolution.o linkerTriblockData.o linkerPECTriblock.o
	mv -iv *.o libdpd.so ./lib/

triblockProcessor:
ifndef HPC	
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -c copolymerMicelleFrame.cpp
	$(CC) $(WARN) $(STDFLAG) -DUSING -o triblockProcessor dpdTrajectory.cpp copolymerMicelleFrame.o micelle.o hydrophobicCore.o bin.o pecTriblock.o polymerBlock.o bead.o vect.o
else
	$(CC) $(WARN) $(STDFLAG) $(LIBDPD) -DUSING -o triblockProcessor dpdTrajectory.cpp
endif

colorTriblock:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -c copolymerMicelleFrame.cpp
	$(CC) $(WARN) $(STDFLAG) -c dpdTrajectory.cpp
	$(CC) $(WARN) $(STDFLAG) -o colorTriblock colorTriblock.cpp dpdTrajectory.o copolymerMicelleFrame.o micelle.o hydrophobicCore.o bin.o pecTriblock.o polymerBlock.o bead.o vect.o
else
	$(CC) $(WARN) $(STDFLAG) $(LIBDPD) -o colorTriblock colorTriblock.cpp
endif

triblockTime:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -c copolymerMicelleFrame.cpp
	$(CC) $(WARN) $(STDFLAG) -c dpdTrajectory.cpp
	$(CC) $(WARN) $(STDFLAG) -DUSING -o triblockTime triblockTimeEvolution.cpp dpdTrajectory.o copolymerMicelleFrame.o micelle.o hydrophobicCore.o bin.o pecTriblock.o polymerBlock.o bead.o vect.o
else
	$(CC) $(WARN) $(STDFLAG) $(LIBDPD) -DUSING -o triblockTime triblockTimeEvolution.cpp
endif

triCoresTime:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c hydrophobicCore.cpp
	$(CC) $(WARN) $(STDFLAG) -c micelle.cpp
	$(CC) $(WARN) $(STDFLAG) -c copolymerMicelleFrame.cpp
	$(CC) $(WARN) $(STDFLAG) -c dpdTrajectory.cpp
	$(CC) $(WARN) $(STDFLAG) -c triblockTimeEvolution.cpp
	$(CC) $(WARN) $(STDFLAG) -DUSING -o triCoresTime triblockCoresTime.cpp triblockTimeEvolution.o dpdTrajectory.o copolymerMicelleFrame.o micelle.o hydrophobicCore.o bin.o pecTriblock.o polymerBlock.o bead.o vect.o
else
	$(CC) $(WARN) $(STDFLAG) $(LIBDPD) -DUSING -o triCoresTime triblockCoresTime.cpp
endif

linkerBuilder:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c linkerPECTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) -c dpdPolymerData.cpp
	$(CC) $(WARN) $(STDFLAG) -DUSING -o linkerBuilder vect.o bead.o polymerBlock.o pecTriblock.o linkerPECTriblock.o bin.o dpdPolymerData.o linkerTriblockData.cpp
else
	$(CC) $(WARN) $(STDFLAG) -DUSING $(LIBDPD) -o linkerBuilder linkerTriblockData.cpp
endif

testLinkerBuilder:
ifndef HPC
	rm -rf *.o
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -DTESTING -c vect.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -c bead.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -DTEST -c polymerBlock.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -c pecTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -c linkerPECTriblock.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -c bin.cpp
	$(CC) $(WARN) $(STDFLAG) $(DEBUG) -c dpdPolymerData.cpp
	$(CC) $(WARN) $(STDFLAG) -DUSING -DTESTING -o linkerBuilder vect.o bead.o polymerBlock.o pecTriblock.o linkerPECTriblock.o bin.o dpdPolymerData.o linkerTriblockData.cpp
else
	$(CC) $(WARN) $(STDFLAG) -DUSING -DTESTING $(LIBDPD) -o linkerBuilder linkerTriblockData.cpp
endif