#include "dpdPolymerData.h"
#include <ctime>
#include <iostream>
#include <string>

int main() {

  // Make this input handling a function within a ChargeTriblockBuilder class which descends from
  // DPDBuilder.
  float bond_length;
  float polymer_volume_fraction;
  float pec_charge_density;
  int temp;
  idx box_length;
  idx pec_length;
  idx tail_length;
  std::string datFile;

  srand( time( NULL ) );
  
  std::cout << "Enter boxlength: ";
  std::cin >> temp;
  box_length = (idx) temp;
  
  std::cout << "Enter bond length: ";
  std::cin >> bond_length;
  
  std::cout << "Enter polymer volume fraction: ";
  std::cin >> polymer_volume_fraction;
  
  std::cout << "Enter polyelectrolyte (pec) block length: ";
  std::cin >> temp;
  pec_length = (idx) temp;
  
  std::cout << "Enter hydrophobic tail length: ";
  std::cin >> temp;
  tail_length = (idx) temp;
  
  std::cout << "Enter pec block charge density: ";
  std::cin >> pec_charge_density;
  
  std::cout << "Enter desired filename: ";
  std::cin >> datFile;
  
  //ChargeTriblockData* t = new ChargeTriblockData( "triblockExample.dat", 36, .1f, .1f, 30, 4, (1.0/3.0) );
  ChargeTriblockData* t = new ChargeTriblockData( datFile, box_length, bond_length, polymer_volume_fraction, 
                                                  pec_length, tail_length, pec_charge_density );
  t->generate();
  
  delete t;

  return 0;
}
