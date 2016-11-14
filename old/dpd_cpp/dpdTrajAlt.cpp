#include <iostream>
#include <string>
#include <cstdio>
#include <fstream> 
#include <cstdlib>
#include <ctime>

// source: http://www.cplusplus.com/reference/cstdio/ftell/
unsigned long numBytesInFile( std::string filename ) {
  FILE * pFile;
  unsigned long size = 0;

  pFile = fopen( filename.c_str(),"rb" );
  if ( pFile == NULL) perror ( "Error opening file" );
  else {
    fseek( pFile, 0, SEEK_END );   // non-portable
    size = ftell( pFile );
    fclose( pFile );
  }
  return size;
}

unsigned int determineNumFrames( std::string* filenames, unsigned int numFiles ) {
    srand(time(NULL));

    int randFile = rand() % numFiles;
    std::cout << "Reading frame from file" << randFile << "..." << std::endl;

    std::string line;
    std::ifstream infile( filenames[ randFile ].c_str() );

    unsigned long bytesPerFrame = 0;

    std::getline( infile, line );

    unsigned int num_atoms = atoi( line.c_str() );

    bytesPerFrame += line.length() + 1; // + 1 for extra byte in newline that is lost when \n is replaced with \0

    for ( int i = 0; i < num_atoms + 1; i++ ) {
        std::getline( infile, line );
        bytesPerFrame += line.length() + 1;
    }


    unsigned long bytesInTraj = 0;
    for ( unsigned int i = 0; i < numFiles; i++ ) {
        bytesInTraj += numBytesInFile( filenames[ i ] );
    }

    std::cout << "Bytes Per Frame: " << bytesPerFrame << std::endl;

    infile.close();

    std::cout << "Bytes In Traj: " << bytesInTraj << std::endl;

    unsigned int numFrames = bytesInTraj / bytesPerFrame;

    // Round to the nearest 100
    if ( numFrames >= 1000 ) {
        numFrames /= 100;
        numFrames *= 100;
    }

    std::cout << "Number of atoms: " << num_atoms << std::endl;

    std::cout << "Number of frames: " << numFrames << std::endl;

    return numFrames;
}

// source for reading input from bash command: 
// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
 int main() {

    std::string* fileNames;

    unsigned int numFiles;

    std::cout << "Enter number of files: ";
    std::cin >> numFiles;

    fileNames = new std::string[ numFiles ];

    for ( unsigned int i = 0; i < numFiles; i++ ) {
        std::cout << "Enter trajectory " << i << " filename: \n";
        std::cin >> fileNames[ i ];
    }

    determineNumFrames( fileNames, numFiles );

    return 0;
}
