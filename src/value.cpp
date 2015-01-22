#include "value.hpp"

namespace lyza { namespace json {

value::string_visitor value::visitor_;

value::string_visitor::ret_type value::string_visitor::operator()(value::null) const
{
    return "null";
}

value::string_visitor::ret_type value::string_visitor::operator()(bool b) const
{
    return b ? "true" : "false";
}

value::string_visitor::ret_type value::string_visitor::operator()(double d) const
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

value::string_visitor::ret_type value::string_visitor::operator()(value::string const& s) const
{
    return "\"" + s + "\"";
}

value::string_visitor::ret_type value::string_visitor::operator()(value::array const& a) const
{
    bool first = true;
    std::string acc;

    acc += "[";
    for (auto& it : a) {
        if (!first)
            acc += ",";
        else
            first = false;
        acc += to_string(it);
    }
    acc += "]";

    return acc;
}

value::string_visitor::ret_type value::string_visitor::operator()(value::object const& o) const
{
    bool first = true;
    std::string acc;

    acc += "{";
    for (auto& it : o) {
        if (!first)
            acc += ",";
        else
            first = false;
        acc += "\"" + it.first + "\"" + ":" + to_string(it.second);
    }
    acc += "}";

    return acc;
}


value::value() : var_(value::null())
{
}

value::~value()
{
}

# define DEF_CTOR(T)\
    value::value(T const& t) : var_(t) { }\
    value::value(T&& t) : var_(std::forward<T>(t)) { }

DEF_CTOR(value::null)
DEF_CTOR(value::array)
DEF_CTOR(value::string)
DEF_CTOR(value::object)

value::value(int i) : var_(static_cast<number>(i)) { }
value::value(number d) : var_(d) { }
value::value(boolean b) : var_(b) { }
value::value(const char* s) : var_(string(s)) { }
value::value(value const& v) : var_(v.var_) { }
value::value(value&& v) : var_(std::move(v.var_)) { }

# define DEF_OP(T)\
    value& value::operator=(T const& t) { var_ = t; return *this; }\
    value& value::operator=(T&& t) { var_ = std::move(t); return *this; }

DEF_OP(value::null)
DEF_OP(value::array)
DEF_OP(value::string)
DEF_OP(value::object)

value& value::operator=(bool b)
{
    var_ = b;
    return *this;
}

value& value::operator=(size_t i)
{
    var_ = static_cast<number>(i);
    return *this;
}

value& value::operator=(int i)
{
    var_ = static_cast<number>(i);
    return *this;
}

value& value::operator=(number d)
{
    var_ = d;
    return *this;
}

value& value::operator=(const char* s)
{
    var_ = string(s);
    return *this;
}

value& value::operator=(value const& v)
{
    var_ = v.var_;
    return *this;
}

value& value::operator=(value&& v)
{
    var_ = std::move(v.var_);
    return *this;
}

value::operator object&()
{
    if (var_.isa<object>()) {
        return var_.get<object>();
    }
    throw bad_cast(to_string(*this));
}

value::operator object const&() const
{
    if (var_.isa<object>()) {
        return var_.get<object>();
    }
    throw bad_cast(to_string(*this));
}

value::operator array&()
{
    if (var_.isa<array>()) {
        return var_.get<array>();
    }
    throw bad_cast(to_string(*this));
}

value::operator array const&() const
{
    if (var_.isa<array>()) {
        return var_.get<array>();
    }
    throw bad_cast(to_string(*this));
}

std::string value::to_string(value const& v)
{
    return v.var_.apply_visitor(visitor_);
}

}}
