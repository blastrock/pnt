#include <pnt.hpp>
#include <vector>

int main()
{
  std::vector<int> v = {-5, -3, 0, 8, 15};

  pnt::writef("The vector is %(~%d, %).\n", v);

  return 0;
}
