#include "dpdTrajectory.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

DPDTrajectory::DPDTrajectory() {
    std::cout << "Enter number of files: ";
    std::cin >> this->numFiles;

    this->fileNames = new std::string[ this->numFiles ];

    for ( unsigned int i = 0; i < this->numFiles; i++ ) {
        std::cout << "Enter trajectory " << i << " filename: \n";
        std::cin >> this->fileNames[ i ];
    }

    this->determineNumFrames();
}

DPDTrajectory::~DPDTrajectory() {
  delete[] this->fileNames;
}

void DPDTrajectory::unlink() {}

void DPDTrajectory::determineNumFrames() {
    srand(time(NULL));

    int randFile = rand() % this->numFiles;
    std::cout << "Reading frame from file" << randFile << "..." << std::endl;

    std::string line;
    std::ifstream infile( this->fileNames[ randFile ].c_str() );

    unsigned long bytesPerFrame = 0;

    std::getline( infile, line );

    this->num_atoms = atoi( line.c_str() );

    bytesPerFrame += line.length() + 1; // + 1 for extra byte in newline that is lost when \n is replaced with \0

    for ( int i = 0; i < this->num_atoms + 1; i++ ) {
        std::getline( infile, line );
        bytesPerFrame += line.length() + 1;
    }

    infile.close();

    unsigned long* bytesInEachFile = new unsigned long[ numFiles ];
    unsigned long bytesInTraj = 0;
    for ( unsigned int i = 0; i < this->numFiles; i++ ) {
        bytesInEachFile[ i ] = this->numBytesInFile( this->fileNames[ i ] );
        bytesInTraj += bytesInEachFile[ i ];
    }

    std::cout << "Bytes Per Frame: " << bytesPerFrame << std::endl;

    std::cout << "Bytes In Traj: " << bytesInTraj << std::endl;

    this->numFrames = bytesInTraj / bytesPerFrame;

    // Round to the nearest 100
    if ( this->numFrames >= 1000 ) {
        this->numFrames /= 100;
        this->numFrames *= 100;
    }

    std::cout << "Number of atoms: " << this->num_atoms << std::endl;

    std::cout << "Number of frames: " << this->numFrames << std::endl;

    // calc start file and offset
    if ( this->numFrames > 1000 ) {
      this->startFrameOffset = this->numFrames - 1000;
      unsigned int frames = 0;
      for ( unsigned int i = 0; i < this->numFiles; i++ ) {
        frames += bytesInEachFile[ i ]/bytesPerFrame;
        if ( this->startFrameOffset < frames ) {
          this->startFrameOffset = this->startFrameOffset - ( frames - bytesInEachFile[ i ]/bytesPerFrame );
          this->startFile = i;
          break;
        }
      }
    } else {
      this->startFrameOffset = 0;
      this->startFile = 0;
    }

    std::cout << "Start file: " << this->fileNames[ this->startFile ] << std::endl;

    std::cout << "Frame offset: " << this->startFrameOffset << std::endl;

}

// source: http://www.cplusplus.com/reference/cstdio/ftell/
unsigned long DPDTrajectory::numBytesInFile( std::string filename ) {
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

int main() {

  DPDTrajectory* traj = new DPDTrajectory();

  delete traj;

  return 0;
}