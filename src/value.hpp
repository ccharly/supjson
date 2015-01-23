#ifndef SUPJSON_JSON_VALUE__
# define SUPJSON_JSON_VALUE__

# include <map>
# include <string>
# include <vector>
# include <iomanip>
# include <sstream>
# include <iostream>
# include <type_traits>

# include "defines.hpp"
# include "variant.hpp"
# include "exceptions.hpp"

namespace supjson {

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
		value(); // by default the value is null
        ~value();

	private:
		class string_visitor {
            public:
                typedef std::string ret_type;

                ret_type operator()(null) const;
                ret_type operator()(bool) const;
                ret_type operator()(double) const;
                ret_type operator()(string const&) const;
                ret_type operator()(array const&) const;
                ret_type operator()(object const&) const;
		};

        static string_visitor visitor_;

	public:
# define DECL_CTOR(T)\
		value(T const& t);\
		value(T&& t);

		DECL_CTOR(null)
		DECL_CTOR(array)
		DECL_CTOR(string)
		DECL_CTOR(object)

		value(int i);
		value(number d);
		value(boolean b);
		value(const char* s);

		value(value const& v);
		value(value&& v);

	public:
# define DECL_OP(T)\
		value& operator=(T const& t);\
		value& operator=(T&& t);

		DECL_OP(null)
		DECL_OP(array)
		DECL_OP(string)
		DECL_OP(object)

		value& operator=(bool b);
		value& operator=(size_t i);
		value& operator=(int i);
		value& operator=(number d);
		value& operator=(const char* s);
		value& operator=(value const& v);
		value& operator=(value&& v);

		operator object&();
		operator object const&() const;
		operator array&();
		operator array const&() const;

	public:
		static std::string to_string(value const& v);

		template <typename T>
		static std::string to_string(T const& v)
		{
			return visitor_(v);
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
        typedef
            variant< null, array, object >
            t;

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

}

 #endif
