#ifndef LYZA_JSON_VALUE__
# define LYZA_JSON_VALUE__

# include <map>
# include <string>
# include <vector>
# include <sstream>

# include "fct/variant.hpp"

namespace lyza { namespace json {

class value {
	public:
		class null {};
		typedef bool boolean;
		typedef double number;
		typedef std::string string;
		typedef std::vector<value> array;
		typedef std::map<std::string, value> object;

	public:
		value()
			: var_(null())
		{
		}

	private:
		struct string_visitor {
			typedef std::string ret_type;

			ret_type operator()(null) const
			{
				return "null";
			}

			ret_type operator()(bool b) const
			{
				return b ? "true" : "false";
			}

			ret_type operator()(double d) const
			{
				std::ostringstream ss;

				ss << d;
				return ss.str();
			}

			ret_type operator()(std::string const& s) const
			{
				return "\"" + s + "\"";
			}

			ret_type operator()(std::vector<value> const& v) const
			{
				bool first = true;
				std::string acc;

				acc += "[";
				for (auto& it : v) {
					if (!first)
						acc += ",";
					else
						first = false;
					acc += it.to_string();
				}
				acc += "]";

				return acc;
			}

			ret_type operator()(std::map<std::string, value> const& m) const
			{
				bool first = true;
				std::string acc;

				acc += "{";
				for (auto& it : m) {
					if (!first)
						acc += ",";
					else
						first = false;
					acc += "\"" + it.first + "\"" + ":" + it.second.to_string();
				}
				acc += "}";

				return acc;
			}
		};

	public:
# define DEF_CTOR(T)\
		value(T const& t) : var_(t) { }\
		value(T&& t) : var_(std::move(t)) { }

		DEF_CTOR(null)
		DEF_CTOR(array)
		DEF_CTOR(string)
		DEF_CTOR(object)

		value(int i) : var_(static_cast<number>(i)) { }
		value(number d) : var_(d) { }
		value(boolean b) : var_(b) { }

		value(const char* s)
			: var_(string(s))
		{
		}

		value(value const& v)
			: var_(v.var_)
		{
		}

		value(value&& v)
			: var_(std::move(v.var_))
		{
		}

	public:
# define DEF_OP(T)\
		value& operator=(T const& t) { var_ = t; return *this; }\
		value& operator=(T&& t) { var_ = std::move(t); return *this; }

		DEF_OP(null)
		DEF_OP(array)
		DEF_OP(string)
		DEF_OP(object)

		value& operator=(int i)
		{
			var_ = static_cast<number>(i);
			return *this;
		}

		value& operator=(number d)
		{
			var_ = d;
			return *this;
		}

		value& operator=(const char* s)
		{
			var_ = string(s);
			return *this;
		}

		value& operator=(value const& v)
		{
			var_ = v.var_;
			return *this;
		}

		value& operator=(value&& v)
		{
			var_ = std::move(v.var_);
			return *this;
		}

	public:
		std::string to_string() const
		{
			return var_.apply_visitor(string_visitor());
		}

		static std::string to_string(value const& v)
		{
			return v.var_.apply_visitor(string_visitor());
		}
	
	public:
		typedef
			functional::variant<
				null,
				std::vector<value>,
				bool,
				std::string,
				number,
				std::map<std::string, value>
			> t;

	private:
		t var_;
};

typedef value::object object;
typedef value::boolean boolean;
typedef value::string string;
typedef value::number number;
typedef value::array array;
typedef value::null null;

}}

#endif
