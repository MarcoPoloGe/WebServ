#include "../includes/Webserv_Includes.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		Ft_error err("Bad arguments");

	/* Start Parsing deracineur de bitume */
	std::cout << "🧚‍ Main_parsing launch" << std::endl;
	std::vector<Config> all_config;
	try {
		main_parsing(av, all_config);
	}
	catch (...) {
		std::cerr << "Error: main_parsing" << std::endl;
	}

//	Network	serv(8080, all_config[0]);
//	serv.run();

	CGI test;
	std::cout << "wesh" << std::endl;
	test.execute();


	return (0);
}
