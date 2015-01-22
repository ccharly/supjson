#include "helpers.hpp"
#include "parser.hpp"

namespace lyza { namespace json {

bool has(object const& o, std::string const& key)
{
	return o.find(key) != o.end();
}

value const& get(object const& o, std::string const& key)
{
	if (has(o, key)) {
		return o.find(key)->second;
	}
	throw missing_key(key);
}

void merge(object& a, object& b)
{
	for (auto& it : b) {
		a.insert(it);
	}
}

void merge(object& a, value& b)
{
	lj::object& bb = b;

	for (auto& it : bb) {
		a.insert(it);
	}
}

object string_to_object(std::string const& js)
{
	return parser::parse(producer::from_string(js));
}

}}
