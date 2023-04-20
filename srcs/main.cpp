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

	Config config1(all_config[0]);
	std::cout << config1.getErrorPages(404) << std::endl;
	std::cout << config1.getErrorNames(200) << std::endl;
	Network	serv(8080, config1);
	serv.run();

	return (0);
}
