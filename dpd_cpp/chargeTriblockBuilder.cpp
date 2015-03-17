#include "dpdPolymerData.h"
#include <ctime>

int main() {

  srand( time( NULL ) );
  ChargeTriblockData* t = new ChargeTriblockData( "triblockExample.dat", 36, .1f, .1f, 30, 4, (1.0/3.0) );
  t->generate();

  return 0;
}