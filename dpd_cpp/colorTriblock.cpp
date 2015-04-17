#include "dpdTrajectory.h"

int main() {
  ColorTriblockTraj* traj = new ColorTriblockTraj();

  traj->process();

  delete traj;

  return 0;
}