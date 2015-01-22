#include <iostream>
#include "json.hpp"

namespace lj = lyza::json;

int main(int argc, char **argv)
{
    if (argc < 2) {
        std::cerr << "usage: " << argv[0] << " <file>.json" << std::endl;
        return 1;
    }

	//lj::producer p = lj::producer::from_string("{\"A\":1}");
	lj::producer p = lj::producer::from_file(argv[1]);

	if (!p.good()) {
		return 1;
	}

	lj::value val = lj::parser::parse(p);
    std::cout << "parsed" << std::endl;
	//lj::object val = lj::parser::parse(p);

	std::cout << lj::value::to_string(val) << std::endl;

    return 0;
}
