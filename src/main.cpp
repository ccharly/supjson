#include <iostream>
#include "value.hpp"

int main()
{
	lyza::json::value v;
	lyza::json::object o;
	lyza::json::value::number n = 10.2;
	lyza::json::value::array a;
	lyza::json::value vv;

	a.push_back(10.2);
	a.push_back(std::string("salut"));
	a.push_back(std::string("café"));
	a.push_back(true);
	a.push_back(false);

	v = a;

	o["salut"] = n;
	o["omg"] = a;
	v = o;

	std::cout << v.to_string() << std::endl;

    return 0;
}
