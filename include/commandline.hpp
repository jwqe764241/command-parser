#pragma once

#include <string>
#include <exception>
#include <map>
#include <vector>
#include <array>
#include <algorithm>
#include <initializer_list>
#include <iostream>

#define COMMANDLINE_NAMESPACE_START namespace commandline {
#define COMMANDLINE_NAMESPACE_END }

COMMANDLINE_NAMESPACE_START

class option_not_found_exception : public std::runtime_error
{
private:
	std::string option_name;

public:
	option_not_found_exception(const std::string& message, const std::string& option_name)
		: std::runtime_error(message), option_name(option_name)
	{
	}

	const std::string get_option_name() const noexcept
	{
		return option_name;
	}
};

class option
{
private:
	std::string name;
	std::string short_name;
	int argument_count;

public:
	option(std::string name, int argument_count)
		: name(name), argument_count(argument_count)
	{
	}
	option(std::string name, std::string short_name, int argument_count)
		: name(name), short_name(short_name), argument_count(argument_count)
	{
	}
	option(std::string name)
		: option(name, 0)
	{
	}
	option(std::string name, std::string short_name)
		: option(name, short_name, 0)
	{
	}

	std::string get_name() const
	{
		return name;
	}

	std::string get_short_name() const
	{
		return short_name;
	}

	int get_argument_count() const
	{
		return argument_count;
	}
};

auto find_option(std::string name, std::vector<commandline::option>& options)
{
	return std::find_if(options.begin(), options.end(), [&name](const option& o){
		return o.get_name() == name || o.get_short_name() == name;
	});
}

class parser
{
private:
	//store parsed options
	std::vector<std::pair<option, std::vector<std::string>>> parsed_option;

private:
	void parse(int argc, char** argv, std::vector<option> options)
	{
		for (int i = 1; i < argc; ++i)
		{
			std::string current_value = argv[i];

			auto it = find_option(current_value, options);
			if (it != options.end())
			{
				option option = *it;

				int range_end = i + 1 + option.get_argument_count();
				if (range_end > argc)
				{
					range_end = argc;
				}

				std::vector<std::string> arguments;

				for (int j = i + 1; j < range_end; ++j)
				{
					std::string argument = argv[j];

					auto it = find_option(argument, options);
					if (it == options.end())
					{
						arguments.push_back(argument);
					}
					else
					{
						break;
					}
				}

				add_option(option, arguments);

				i += arguments.size();
			}
		}
	}

	auto find(std::string name) const
	{
		return std::find_if(parsed_option.begin(), parsed_option.end(), [&name](const std::pair<option, std::vector<std::string>>& pair){
			const option& o = pair.first;
			return o.get_name() == name || o.get_short_name() == name;
		});
	}

	void add_option(option o, std::vector<std::string> values)
	{
		assert(!has_option(o.get_name()));
		parsed_option.push_back(std::pair<option, std::vector<std::string>>(o, values));
	}

public:
	parser(int argc, char** argv, std::initializer_list<option> options)
	{
		parse(argc, argv, options);
	}

	bool has_option(std::string name) const
	{
		auto it = find(name);

		return it != parsed_option.end();
	}

	std::vector<std::string> get_arguments(std::string name) const
	{
		auto it = find(name);

		if (it != parsed_option.end())
		{
			return it->second;
		}
		else
		{
			throw option_not_found_exception("can't find " + name, name);
		}
	}

	int get_argument_size(std::string name) const
	{
		auto it = find(name);

		if (it != parsed_option.end())
		{
			return it->second.size();
		}
		else
		{
			throw option_not_found_exception("can't find " + name, name);
		}
	}
};

COMMANDLINE_NAMESPACE_END