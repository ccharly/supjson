#ifndef LYZA_JSON_PARSER__
# define LYZA_JSON_PARSER__

# include <istream>
# include <sstream>
# include <stdexcept>
# include <cmath>

# include "value.hpp"

namespace lyza { namespace json { namespace parser {

namespace lj = lyza::json;

void error(lj::producer& p, std::string const& msg)
{
	std::string until_eof;

	while (!p.eof()) {
		until_eof += p.nextc();
	}

	throw std::runtime_error(msg + "(" + until_eof + ")");
}

bool has(lj::producer& p, char c)
{
	if (p.peekc() == c) {
		return true;
	}
	return false;
}

bool may_have(lj::producer& p, char c)
{
	if (p.peekc() == c) {
		p.nextc();
		return true;
	}
	return false;
}

void expects(lj::producer& p, char c)
{
	char cc = p.nextc();
	if (cc != c) {
		throw std::runtime_error(
				"parse error: '"
				+ std::string(c, 1)
				+ "' expected, got: '"
				+ std::string(cc, 1)
				+ "'");
	}
}

void match_string(lj::producer&p, std::string const& s)
{
	for (auto c : s) {
		expects(p, c);
	}
}

lj::string parse_string(lj::producer& p);
lj::array parse_array(lj::producer& p);
lj::boolean parse_bool(lj::producer& p);
lj::null parse_null(lj::producer& p);
lj::object parse_object(lj::producer& p);
lj::value parse_value(lj::producer& p);

lj::string parse_string(lj::producer& p)
{
	std::cout << "parse_string" << std::endl;
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

lj::boolean parse_bool(lj::producer& p)
{
	lj::boolean b;

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

lj::null parse_null(lj::producer& p)
{
	if (has(p, 'n')) {
		match_string(p, "null");
	} else {
		error(p, "expected null");
	}
	return lj::null();
}

lj::array parse_array(lj::producer& p)
{
	lj::array a;

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

lj::object parse_object(lj::producer& p)
{
	lj::object o;

	expects(p, '{');
	if (has(p, '}')) { // empty
		p.nextc();
		return o;
	}

	bool more = true;
	while (more) {
		if (has(p, '"')) {
			lj::string key = parse_string(p);
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

lj::number parse_number(lj::producer& p)
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
		while (std::isdigit(p.peekc())) {
			num += p.nextc();
		}
	}

	if (has(p, '.')) {
		num += p.nextc();
	}

	if (std::isdigit(p.peekc())) {
		num += p.nextc();
	} else {
		error(p, "expected number");
	}

	while (std::isdigit(p.peekc())) {
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
		while (std::isdigit(p.peekc())) {
			exp += p.nextc();
		}

		double dexp = std::pow(10, (op == '-' ? -1. : 1.) * std::stod(exp));

		dnum = dnum * dexp;
	}

	return dnum;
}

lj::value parse_value(lj::producer& p)
{
	std::cout << "parse_value" << std::endl;
	lj::value val;

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
	} else if (has(p, '-') || std::isdigit(p.peekc())) {
		val = parse_number(p);
	} else {
		error(p, "expected value");
	}
	return val;
}

lj::object parse(lj::producer& p)
{
	p.skip_ws(true);
	return parse_object(p);
}

}}}

#endif
