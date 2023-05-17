#include "../includes/Webserv_Includes.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		Ft_error err("Bad arguments");

	/* Start Parsing deracineur de bitume */
	std::cout <<B<< "🧚‍ Main_parsing launch" <<RE<< std::endl;
	std::vector<Config> all_config;
	try {
		main_parsing(av, all_config);
	}
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		exit (1);
	}
	std::cout <<G<< "🌸 Main_parsing clear" <<RE<< std::endl;

	LAN	all_servers(all_config);
	all_servers.runAll();

	return (0);
}
