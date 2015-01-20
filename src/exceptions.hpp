#ifndef LYZA_JSON_EXN__
# define LYZA_JSON_EXN__

# include <string>
# include <stdexcept>

namespace lyza { namespace json {

class missing_key : public std::runtime_error {
	std::string key_;

	public:
		missing_key(std::string const& key)
			: std::runtime_error(std::string("missing key: " + key)), key_(key)
		{
		}

		std::string const& which_key() const
		{
			return key_;
		}
};

class bad_cast : public std::bad_cast {
	std::string json_;

	public:
		bad_cast(std::string const& js)
			: std::bad_cast(), json_(js)
		{
		}

		virtual const char* what() const throw()
		{
			return json_.c_str();
		}
};

class end_of_file : public std::runtime_error {
	public:
		end_of_file()
			: std::runtime_error("eof")
		{
		}
};

}}

#endif
