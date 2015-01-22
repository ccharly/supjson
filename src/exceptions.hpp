#ifndef LYZA_JSON_EXN__
# define LYZA_JSON_EXN__

# include <string>
# include <sstream>
# include <stdexcept>

namespace lyza { namespace json {

class parse_error : public std::runtime_error {
    std::string what_;

    public:
    explicit parse_error(size_t line, size_t column, const std::string& what_arg) : std::runtime_error("") {
        std::stringstream ss;

        ss << line << ':' << column << ": " << what_arg;
        ss << "\n";

        what_ = ss.str();
    }

    virtual const char* what() const throw() {
        return what_.c_str();
    }
};

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

class bad_visit : public std::runtime_error {
    public:
        bad_visit(size_t tag)
            : std::runtime_error((std::stringstream("bad visit (tag was: ") << tag).str())
        {
        }
        ~bad_visit() {}
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
