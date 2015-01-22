#ifndef LYZA_JSON_PARSER__
# define LYZA_JSON_PARSER__

# include <istream>
# include <sstream>
# include <stdexcept>
# include <locale>
# include <cmath>

# include "producer.hpp"
# include "value.hpp"

namespace lyza { namespace json {

namespace lj = lyza::json;

class parser {
	private:
        static void error(lj::producer& p, std::string const& msg);
        static bool has(lj::producer& p, char c);
        static bool may_have(lj::producer& p, char c);
        static std::string char_to_code(char c);
        static void expects(lj::producer& p, char c);
        static void match_string(lj::producer&p, std::string const& s);

	public:
        static lj::string parse_string(lj::producer& p);
        static lj::boolean parse_bool(lj::producer& p);
        static lj::null parse_null(lj::producer& p);
        static lj::array parse_array(lj::producer& p);
        static lj::object parse_object(lj::producer& p);
        static lj::number parse_number(lj::producer& p);
        static lj::value parse_value(lj::producer& p);
        static lj::object parse(lj::producer& p);
        static lj::object parse(lj::producer&& p);

};

}}

#endif
