#ifndef SUPJSON_JSON_PARSER__
# define SUPJSON_JSON_PARSER__

# include <istream>
# include <sstream>
# include <stdexcept>
# include <locale>
# include <cmath>

# include "producer.hpp"
# include "value.hpp"

namespace supjson {

class parser {
	private:
        static void error(supjson::producer& p, std::string const& msg);
        static bool has(supjson::producer& p, char c);
        static bool may_have(supjson::producer& p, char c);
        static std::string char_to_code(char c);
        static void expects(supjson::producer& p, char c);
        static void match_string(supjson::producer&p, std::string const& s);

	public:
        static supjson::string parse_string(supjson::producer& p);
        static supjson::boolean parse_bool(supjson::producer& p);
        static supjson::null parse_null(supjson::producer& p);
        static supjson::array parse_array(supjson::producer& p);
        static supjson::object parse_object(supjson::producer& p);
        static supjson::number parse_number(supjson::producer& p);
        static supjson::value parse_value(supjson::producer& p);
        static supjson::value parse(supjson::producer& p);
        static supjson::value parse(supjson::producer&& p);

};

}

#endif
