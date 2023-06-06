#include "../includes/Webserv_Includes.hpp"

int	main(int ac, char **av)
{
	if (ac < 1 || 2 < ac)
		Ft_error err("Bad arguments");

	/* Start Parsing deracineur de bitume */
	std::cout <<B<< "🧚‍ Main_parsing launch" <<RE<< std::endl;
	std::vector<Config> all_config;
	try {
		if (ac == 1)
			main_parsing("config/config.conf", all_config);
		else
			main_parsing(av, all_config);
	}
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		exit (1);
	}
	std::cout <<G<< "🌸 Main_parsing clear" <<RE<< std::endl << std::endl;

	try {
		LAN	all_servers(all_config);
		all_servers.runAll();
	}
	catch (std::invalid_argument &e) {
		std::cout <<R<< "An error occured on webserv lauching :\n" << e.what() << "\n" <<RE;
	}

	std::exit(0);
	return (0);
}
