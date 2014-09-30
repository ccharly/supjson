#include <iostream>
#include "json.hpp"

namespace lj = lyza::json;

int main()
{
	lj::producer p = lj::producer::from_file("data.json");

	if (!p.good()) {
		return 1;
	}

	lj::value val = lj::parser::parse(p);

	std::cout << val.to_string() << std::endl;


    return 0;
}
