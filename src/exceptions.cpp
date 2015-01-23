#include "exceptions.hpp"

namespace lyza { namespace json {

parse_error::parse_error(size_t line, size_t column, const std::string& what_arg)
    : std::runtime_error("") {
        std::stringstream ss;

        ss << line << ':' << column << ": " << what_arg;
        ss << "\n";

        what_ = ss.str();
    }

const char* parse_error::what() const throw() {
    return what_.c_str();
}

missing_key::missing_key(std::string const& key)
    : std::runtime_error(std::string("missing key: " + key)), key_(key) {
}

std::string const& missing_key::which_key() const {
    return key_;
}

bad_cast::bad_cast(std::string const& js)
    : std::bad_cast(), json_(js) {
}

const char* bad_cast::what() const throw() {
    return json_.c_str();
}

bad_visit::bad_visit(size_t tag)
    : std::runtime_error((std::stringstream("bad visit (tag was: ") << tag).str()) {
}

end_of_file::end_of_file()
    : std::runtime_error("eof") {
}

}}
