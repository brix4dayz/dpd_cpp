#include "dpdTrajectory.h"

int main() {
  
  std::unique_ptr<ColorTriblockTraj> traj( new ColorTriblockTraj() );

  if (traj)
    traj->process();

  traj.reset();

  return 0;
}