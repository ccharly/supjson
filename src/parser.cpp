#include "parser.hpp"

namespace {
	bool isdigit(char c)
	{
		// TODO Should we instanciate the locate every times ?
		static std::locale loc;
		return std::isdigit(c, loc);
	}
}

namespace supjson {

void parser::error(supjson::producer& p, std::string const& msg)
{
    std::string until_eof;

    while (!p.eof()) {
        until_eof += p.nextc();
    }

    throw parse_error(p.get_line(), p.get_column(), msg + " (until_eof=\"" + until_eof + "\")");
}

bool parser::has(supjson::producer& p, char c)
{
    return p.peekc() == c;
}

bool parser::may_have(supjson::producer& p, char c)
{
    if (p.peekc() == c) {
        p.nextc();
        return true;
    }
    return false;
}

std::string parser::char_to_code(char c)
{
    std::stringstream ss;

    ss << static_cast<int>(c);
    return ss.str();
}

void parser::expects(supjson::producer& p, char c)
{
    char cc = p.nextc();
    if (cc != c) {
        throw parse_error(p.get_line(), p.get_column(),
                std::string("parse error: ")
                + "'" + std::string(c, 1) + "'"
                + "(code=" + char_to_code(c) + ")"
                + "	expected, got: "
                + "'" + std::string(cc, 1) + "'"
                + "' (code=" + char_to_code(cc) + ")");
    }
}

void parser::match_string(supjson::producer&p, std::string const& s)
{
    for (auto c : s) {
        expects(p, c);
    }
}

supjson::string parser::parse_string(supjson::producer& p)
{
    std::string acc;

    expects(p, '"');
    p.skip_ws(false);
    while (!has(p, '"')) {
        if (may_have(p, '\\')) {
            acc += p.nextc();
        } else {
            acc += p.nextc();
        }
    }
    expects(p, '"');
    p.skip_ws(true);

    return acc;
}

supjson::boolean parser::parse_bool(supjson::producer& p)
{
    supjson::boolean b = false;

    if (has(p, 't')) {
        match_string(p, "true");
        b = true;
    } else if (has(p, 'f')) {
        match_string(p, "false");
        b = false;
    } else {
        error(p, "expected boolean");
    }
    return b;
}

supjson::null parser::parse_null(supjson::producer& p)
{
    if (has(p, 'n')) {
        match_string(p, "null");
    } else {
        error(p, "expected null");
    }
    return supjson::null();
}

supjson::array parser::parse_array(supjson::producer& p)
{
    supjson::array a;

    expects(p, '[');
    if (has(p, ']')) { // empty
        p.nextc();
        return a;
    }

    bool more = true;
    while (more) {
        a.push_back(parse_value(p));
        if (has(p, ','))  {
            p.nextc();
            more = true;
        } else
            more = false;
    }
    expects(p, ']');

    return a;
}

supjson::object parser::parse_object(supjson::producer& p)
{
    supjson::object o;

    expects(p, '{');
    if (has(p, '}')) { // empty
        p.nextc();
        return o;
    }

    bool more = true;
    while (more) {
        if (has(p, '"')) {
            supjson::string key = parse_string(p);
            expects(p, ':');
            o[key] = parse_value(p);
            if (has(p, ','))  {
                p.nextc();
                more = true;
            } else
                more = false;
        } else {
            error(p, "expected key");
        }
    }
    expects(p, '}');
    return o;
}

supjson::number parser::parse_number(supjson::producer& p)
{
    bool neg = false;

    if (has(p, '-')) {
        p.nextc();
        neg = true;
    }

    std::string num;
    if (has(p, '0')) {
        num += p.nextc();
    } else {
        if ('0' < p.peekc() && p.peekc() <= '9') {
            num += p.nextc();
        } else {
            error(p, "expected number");
        }
        while (isdigit(p.peekc())) {
            num += p.nextc();
        }
    }

    if (has(p, '.')) {
        num += p.nextc();

        if (isdigit(p.peekc())) {
            num += p.nextc();
        } else {
            error(p, "expected number");
        }
    }


    while (isdigit(p.peekc())) {
        num += p.nextc();
    }

    double dnum = std::stod(num);

    if (has(p, 'e') || has(p, 'E')) {
        p.nextc();

        char op = '+';
        if (has(p, '-') || has(p, '+')) {
            op = p.nextc();
        }

        std::string exp;
        while (isdigit(p.peekc())) {
            exp += p.nextc();
        }

        double dexp = std::pow(10, (op == '-' ? -1. : 1.) * std::stod(exp));

        dnum = dnum * dexp;
    }

    return neg ? -dnum : dnum;
}

supjson::value parser::parse_value(supjson::producer& p)
{
    supjson::value val;

    if (has(p, '"')) {
        val = parse_string(p);
    } else if (has(p, '[')) {
        val = parse_array(p);
    } else if (has(p, '{')) {
        val = parse_object(p);
    } else if (has(p, 't') || has(p, 'f')) {
        val = parse_bool(p);
    } else if (has(p, 'n')) {
        val = parse_null(p);
    } else if (has(p, '-') || isdigit(p.peekc())) {
        val = parse_number(p);
    } else {
        error(p, "expected value");
    }
    return val;
}

supjson::value parser::parse(supjson::producer& p)
{
    p.skip_ws(true);
    return parse_value(p);
}

supjson::value parser::parse(supjson::producer&& p)
{
    p.skip_ws(true);
    return parse_value(p);
}

}
