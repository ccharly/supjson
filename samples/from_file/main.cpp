#include <iostream>
#include "json.hpp"

namespace lj = lyza::json;

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <file>.json" << std::endl;
        return 1;
    }

	lj::producer p = lj::producer::from_file(argv[1]);

	if (!p.good()) {
        std::cerr << "error while opening " << argv[1] << std::endl;
		return 1;
	}

    try {
        std::cout <<
            lj::value::to_string(
                    lj::parser::parse(p))
            << std::endl;
    } catch (lj::parse_error const& e) {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
