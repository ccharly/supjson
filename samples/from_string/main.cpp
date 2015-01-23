#include <iostream>
#include "json.hpp"

namespace sj = supjson;

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <string_json>" << std::endl;
        return 1;
    }

    try {
        std::cout <<
            sj::value::to_string(
                    sj::parser::parse(sj::producer::from_string(argv[1])))
            << std::endl;
    } catch (sj::parse_error const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
