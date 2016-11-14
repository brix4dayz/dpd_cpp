#include <functional>
#include <random>
#include <memory>

template <typename T>
class IntegerDice {
  private:
    std::function< T() > dice;
  public:
    T roll() { return dice(); }
    ~IntegerDice() {}
    IntegerDice( T min, T max ) {
      std::random_device seed;
      std::mt19937 gen( seed() );
      std::uniform_int_distribution< T > dist( min, max );
      dice = std::bind( dist, gen );
    }
};

template <typename T>
class RealDice {
  private:
    std::function< T() > dice;
  public:
    T roll() { return dice(); }
    ~RealDice() {}
    RealDice( T min, T max ) {
      std::random_device seed;
      std::mt19937 gen( seed() );
      std::uniform_real_distribution< T > dist( min, max );
      dice = std::bind( dist, gen );
    }
};