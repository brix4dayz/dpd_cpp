#include <iostream>
#include <string>
#include <cstdio>
#include <fstream> 
#include <cstdlib>

// source for reading input from bash command: 
// http://stackoverflow.com/questions/478898/how-to-execute-a-command-and-get-output-of-command-within-c
 int main() {

    std::string filename;

    std::cout << "Enter trajectory filename: ";
    std::cin >> filename;

    char* cmd = new char[ 27 ];

    sprintf( cmd, "wc -l %20s", filename.c_str() );

    FILE* pipe = popen(cmd, "r");
    if (!pipe) return 0;
    char buffer[128];
    std::string result = "";
    while(!feof(pipe)) {
      if(fgets(buffer, 128, pipe) != NULL)
        result += buffer;
    }
    pclose(pipe);

    delete[] cmd;

    int numLines = atoi( result.c_str() );

    std::cout << "File contains " << numLines << " lines." << std::endl;

    std::string line;
    std::ifstream infile( filename.c_str() );

    std::getline( infile, line );

    unsigned int num_atoms = atoi( line.c_str() );

    std::cout << "Number of atoms: " << num_atoms << std::endl;

    unsigned short numFrames = ( numLines )/( num_atoms + 2 );

    std::cout << "Number of frames: " << numFrames << std::endl;

    infile.close();

    return 0;
}
