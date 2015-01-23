#include <iostream>
#include "json.hpp"

namespace lj = lyza::json;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <string_json>" << std::endl;
        return 1;
    }

    try {
        std::cout <<
            lj::value::to_string(
                    lj::parser::parse(lj::producer::from_string(argv[1])))
            << std::endl;
    } catch (lj::parse_error const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
