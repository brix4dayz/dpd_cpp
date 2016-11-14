#include "../Test1/position.h"

int main() {
  
  position* p = new position();
  p->x = 0;
  p->y = 0;
  p->z = 0;

  std::cout << p->x << " " << p->y << " " << p->z << std::endl;

  return 0;
}