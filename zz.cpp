#include <iostream>

extern "C" int func(const int &a, const double &b)
{
    std::cout << a << " " << b << std::endl;
    std::cout << "Je suis dans une dll!" << std::endl;

    return 120;
}
