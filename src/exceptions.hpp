#ifndef LYZA_JSON_EXN__
# define LYZA_JSON_EXN__

# include <string>
# include <sstream>
# include <stdexcept>

namespace lyza { namespace json {

class parse_error : public std::runtime_error {
    std::string what_;

    public:
    explicit parse_error(size_t line, size_t column, const std::string& what_arg);
    virtual const char* what() const throw();
};

class missing_key : public std::runtime_error {
	std::string key_;

	public:
		missing_key(std::string const& key);
		std::string const& which_key() const;
};

class bad_cast : public std::bad_cast {
	std::string json_;

	public:
		bad_cast(std::string const& js);
		virtual const char* what() const throw();
};

class bad_visit : public std::runtime_error {
    public:
        bad_visit(size_t tag);
};

class end_of_file : public std::runtime_error {
	public:
		end_of_file();
};

}}

#endif
