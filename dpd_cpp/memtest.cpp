#include <iostream>

class Guy {
  public:
    float* r;
    Guy() {}
    ~Guy() {
      delete r;
    }
};

void doNothing( int* thing ) {

}

int main() {
  int* temp = new int(5);

  Guy* firend = new Guy();

  firend->r = new float(5.5);

  doNothing( temp );

  printf("%f\n", *(firend->r));

  delete firend;
  delete temp;

  return 0;
}

