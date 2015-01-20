#ifndef LYZA_JSON_VALUE__
# define LYZA_JSON_VALUE__

# include <map>
# include <string>
# include <vector>
# include <iomanip>
# include <sstream>
# include <iostream>
# include <type_traits>

# include "defines.hpp"
# include "fct/variant.hpp"
# include "exceptions.hpp"

namespace lyza { namespace json {

template <typename T>
struct type_matcher {
	typedef T type;
};

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
				std::stringstream ss;

				ss << std::fixed << d;
				// FIXME, std::noshowpoints collide with std::fixed, so I do
				// it by myself..

				bool start = false;
				bool only0 = true;

				std::string bef;
				std::string aft;

				std::string* in_use = &bef;
				for (auto c : ss.str()) {
					if (c == '.') {
						start = true;
						in_use = &aft;
					} else {
						if (only0 && start && c != '0')
							only0 = false;
						*in_use += c;
					}
				}

				return bef + (only0 ? "" : "." + aft);
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
					acc += to_string(it);
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
					acc += "\"" + it.first + "\"" + ":" + to_string(it.second);
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

		value& operator=(bool b)
		{
			var_ = b;
			return *this;
		}

		value& operator=(size_t i)
		{
			var_ = static_cast<number>(i);
			return *this;
		}

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

		operator object&()
		{
			if (var_.isa<object>()) {
				return var_.get<object>();
			}
			std::cout << "object" << std::endl;
			throw bad_cast(to_string(*this));
		}

		operator object const&() const
		{
			if (var_.isa<object>()) {
				return var_.get<object>();
			}
			std::cout << "object" << std::endl;
			throw bad_cast(to_string(*this));
		}

		operator array&()
		{
			if (var_.isa<array>()) {
				return var_.get<array>();
			}
			std::cout << "array" << std::endl;
			throw bad_cast(to_string(*this));
		}

		operator array const&() const
		{
			if (var_.isa<array>()) {
				return var_.get<array>();
			}
			std::cout << "array" << std::endl;
			throw bad_cast(to_string(*this));
		}

	private:
		static string_visitor& get_visitor()
		{
			static string_visitor v;
			return v;
		}

	public:
		static std::string to_string(value const& v)
		{
			return v.var_.apply_visitor(get_visitor());
		}

		template <typename T>
		static std::string to_string(T const& v)
		{
			return get_visitor()(v);
		}

	public:
		template <typename T>
		bool isa() const
		{
			return var_.isa<T>();
		}

	public:
		template <typename T>
		T& get()
		{
			return static_cast<T&>(var_.get<typename type_matcher<T>::type>());
		}

		template <typename T>
		const T& get() const
		{
			return static_cast<const T&>(var_.get<typename type_matcher<T>::type>());
		}

	public:
# ifdef NO_VTEMPLATES
		typedef
			functional::variant<
				mp::tlist< null,
				mp::tlist< std::vector<value>,
				mp::tlist< bool,
				mp::tlist< std::string,
				mp::tlist< number,
				mp::tlist< std::map<std::string, value>,
				mp::null
					>
					>
					>
					>
					>
					>
			> t;
# else
		typedef
			functional::variant<
				null,
				std::vector<value>,
				bool,
				std::string,
				number,
				std::map<std::string, value>
			> t;
# endif

	public:
		t var_;
};

typedef value::object object;
typedef value::boolean boolean;
typedef value::string string;
typedef value::number number;
typedef value::array array;
typedef value::null null;

template <>
struct type_matcher<int> {
	typedef number type;
};

template <>
struct type_matcher<float> {
	typedef number type;
};

}}

#endif
