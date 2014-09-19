#include <unistd.h>
#include <iostream>
#include "producer.hpp"

int main()
{
    lyza::json::producer p = lyza::json::producer::from_string("salut");

    while (!p.eof()) {
        std::cout << static_cast<int>(p.peekc()) << ":";
        std::cout << p.nextc() << std::endl;
    }

    return 0;
}
