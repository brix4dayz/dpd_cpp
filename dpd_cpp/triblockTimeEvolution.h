#include "dpdTrajectory.h"

class TriblockTimeEvolution : public TriblockTrajectory {
  public:
    TriblockTimeEvolution();
    ~TriblockTimeEvolution();
    void analyzeHelp( std::ifstream& inFile, FILE* fp );
    void setupHelp( FILE* fp );
    void calcHelp();
    void process();
};