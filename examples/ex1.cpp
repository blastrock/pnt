#include <xll/pnt.hpp>
#include <iostream>
#include <iomanip>

int main()
{
    int value = 12345;

    std::cout << "The value is " << std::setw(10) << std::setfill('0') << std::showpos << std::internal << value << " which is, in hexa, " << std::setw(8) << std::setfill('0') << std::showbase << std::hex << value << '.' << std::endl;

    xll::pnt::writef("The value is %0$+010d which is, in hexa, %0$#08x.\n", value);

    return 0;
}
