////////////////////////////////////////////////////////////
// Nero Game Engine
// Copyright (c) 2016-2020 Sanou A. K. Landry
////////////////////////////////////////////////////////////
///////////////////////////HEADERS//////////////////////////
////////////////////////////////////////////////////////////
#include <iostream>

#include <cxxopts/cxxopts.hpp>

void
parse(int argc, char* argv[])
{
  try
  {
	cxxopts::Options options(argv[0], " - example command line options");
	options
	  .positional_help("[optional args]")
	  .show_positional_help();

	bool apple = false;

	options
	  .allow_unrecognised_options()
	  .add_options()
	  ("a,apple", "an apple", cxxopts::value<bool>(apple))
	  ("b,bob", "Bob")
	  ("char", "A character", cxxopts::value<char>())
	  ("t,true", "True", cxxopts::value<bool>()->default_value("true"))
	  ("f, file", "File", cxxopts::value<std::vector<std::string>>(), "FILE")
	  ("i,input", "Input", cxxopts::value<std::string>())
	  ("o,output", "Output file", cxxopts::value<std::string>()
		  ->default_value("a.out")->implicit_value("b.def"), "BIN")
	  ("positional",
		"Positional arguments: these are the arguments that are entered "
		"without an option", cxxopts::value<std::vector<std::string>>())
	  ("long-description",
		"thisisareallylongwordthattakesupthewholelineandcannotbebrokenataspace")
	  ("help", "Print help")
	  ("int", "An integer", cxxopts::value<int>(), "N")
	  ("float", "A floating point number", cxxopts::value<float>())
	  ("vector", "A list of doubles", cxxopts::value<std::vector<double>>())
	  ("option_that_is_too_long_for_the_help", "A very long option")
	#ifdef CXXOPTS_USE_UNICODE
	  ("unicode", u8"A help option with non-ascii: à. Here the size of the"
		" string should be correct")
	#endif
	;

	options.add_options("Group")
	  ("c,compile", "compile")
	  ("d,drop", "drop", cxxopts::value<std::vector<std::string>>());

	options.parse_positional({"input", "output", "positional"});

	auto result = options.parse(argc, argv);

	if (result.count("help"))
	{
	  std::cout << options.help({"", "Group"}) << std::endl;
	  exit(0);
	}

	if (apple)
	{
	  std::cout << "Saw option ‘a’ " << result.count("a") << " times " <<
		std::endl;
	}

	if (result.count("b"))
	{
	  std::cout << "Saw option ‘b’" << std::endl;
	}

	if (result.count("char"))
	{
	  std::cout << "Saw a character ‘" << result["char"].as<char>() << "’" << std::endl;
	}

	if (result.count("f"))
	{
	  auto& ff = result["f"].as<std::vector<std::string>>();
	  std::cout << "Files" << std::endl;
	  for (const auto& f : ff)
	  {
		std::cout << f << std::endl;
	  }
	}

	if (result.count("input"))
	{
	  std::cout << "Input = " << result["input"].as<std::string>()
		<< std::endl;
	}

	if (result.count("output"))
	{
	  std::cout << "Output = " << result["output"].as<std::string>()
		<< std::endl;
	}

	if (result.count("positional"))
	{
	  std::cout << "Positional = {";
	  auto& v = result["positional"].as<std::vector<std::string>>();
	  for (const auto& s : v) {
		std::cout << s << ", ";
	  }
	  std::cout << "}" << std::endl;
	}

	if (result.count("int"))
	{
	  std::cout << "int = " << result["int"].as<int>() << std::endl;
	}

	if (result.count("float"))
	{
	  std::cout << "float = " << result["float"].as<float>() << std::endl;
	}

	if (result.count("vector"))
	{
	  std::cout << "vector = ";
	  const auto values = result["vector"].as<std::vector<double>>();
	  for (const auto& v : values) {
		std::cout << v << ", ";
	  }
	  std::cout << std::endl;
	}

	std::cout << "Arguments remain = " << argc << std::endl;

	auto arguments = result.arguments();
	std::cout << "Saw " << arguments.size() << " arguments" << std::endl;
  }
  catch (const cxxopts::OptionException& e)
  {
	std::cout << "error parsing options: " << e.what() << std::endl;
	exit(1);
  }
}

int main(int argc, char* argv[])
{
  parse(argc, argv);

  return 0;
}
