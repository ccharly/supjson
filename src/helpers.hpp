#ifndef SUPJSON_JSON_HELPERS__
# define SUPJSON_JSON_HELPERS__

# include <string>

# include "value.hpp"

namespace supjson {

bool has(object const& o, std::string const& key);
value const& get(object const& o, std::string const& key);
void merge(object& a, object& b);
void merge(object& a, value& b);
object string_to_object(std::string const& js);

template <typename T>
const T& get(object const& o, std::string const& key)
{
	return o.find(key)->second.template get<T>();
}

}

#endif
