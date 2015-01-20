#ifndef LYZA_JSON_HELPERS__
# define LYZA_JSON_HELPERS__

# include <string>

# include "value.hpp"
# include "parser.hpp"

namespace lyza { namespace json {

static bool has(object const& o, std::string const& key)
{
	return o.find(key) != o.end();
}

static value const& get(object const& o, std::string const& key)
{
	if (has(o, key)) {
		return o.find(key)->second;
	}
	throw std::runtime_error("expecting the following key: " + key);
}

static void merge(object& a, object& b)
{
	for (auto& it : b) {
		a.insert(it);
	}
}

static void merge(object& a, value& b)
{
	lj::object& bb = b;

	for (auto& it : bb) {
		a.insert(it);
	}
}

static object string_to_object(std::string const& js)
{
	return parser::parse(producer::from_string(js));
}

template <typename T>
const T& get(object const& o, std::string const& key)
{
	return o.find(key)->second.template get<T>();
}

}}

#endif
