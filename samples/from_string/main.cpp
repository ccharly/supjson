#include <iostream>
#include "json.hpp"

namespace lj = lyza::json;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <string_json>" << std::endl;
        return 1;
    }

    lj::value val = lj::parser::parse(lj::producer::from_string(argv[1]));
    std::cout << lj::value::to_string(val) << std::endl;

    return 0;
}
