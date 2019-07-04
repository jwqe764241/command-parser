#include "line_option.hpp"

line_option::line_option(name_pair name, int argument_count)
	: name(name), argument_count(argument_count)
{
}

line_option::line_option(std::string prefix, std::string name, int argument_count)
	: line_option(std::make_pair(prefix, name), argument_count)
{
}

line_option::line_option(name_pair name, name_pair short_name, int argument_count)
	: name(name), short_name(short_name), argument_count(argument_count)
{
}

line_option::line_option(std::string prefix, std::string name,
	std::string short_prefix, std::string short_name, int argument_count)
	: line_option(std::make_pair(prefix, name), std::make_pair(short_prefix, short_name), argument_count)
{
}

std::string line_option::get_prefix() const
{
	return name.first;
}

std::string line_option::get_name() const
{
	return name.second;
}

std::string line_option::get_short_prefix() const
{
	return short_name.first;
}

std::string line_option::get_short_name() const
{
	return short_name.second;
}

std::string line_option::get_real_name() const
{
	return name.first + name.second;
}

std::string line_option::get_real_short_name() const
{
	return short_name.first + short_name.second;
}

int line_option::get_argument_count() const
{
	return argument_count;
}