#include "triblockTimeEvolution.h"

class TriblockCoresTime : public TriblockTimeEvolution {
  public:
    TriblockCoresTime() : TriblockTimeEvolution() {}
    void analyzeHelp( std::ifstream& inFile, FILE* fp );
    void setupHelp( FILE* fp );
};