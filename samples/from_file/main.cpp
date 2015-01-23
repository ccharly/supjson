#include <iostream>
#include "json.hpp"

namespace sj = supjson;

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <file>.json" << std::endl;
        return 1;
    }

	sj::producer p = sj::producer::from_file(argv[1]);

	if (!p.good()) {
        std::cerr << "error while opening " << argv[1] << std::endl;
		return 1;
	}

    try {
        std::cout <<
            sj::value::to_string(
                    sj::parser::parse(p))
            << std::endl;
    } catch (sj::parse_error const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
