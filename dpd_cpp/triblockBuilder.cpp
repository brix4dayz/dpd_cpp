#include "dpdPolymerData.h"

int main() {

  TriblockData* t = new TriblockData( "triblockExample.dat", 36, .1f, .1f, 50, 4 );
  t->generate();

  return 0;
}