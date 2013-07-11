#include <pnt.hpp>
#include <vector>

int main()
{
  pnt::writef("Here is the list: %([%#x]%|, %).\n", std::vector<int>{1, 2, 3, 4});

  std::vector<int> v = {-5, -3, 0, 8, 15};

  pnt::writef("The vector is %(%d, %).\n", v);
  pnt::writef("The vector is %(~%d~%|, %).\n", v);
  pnt::writef("The vector is %(%%|%%%d%%|%%%| %).\n", v);

  return 0;
}
