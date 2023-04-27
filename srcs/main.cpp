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
		handle_multiport(all_config);
	}
	catch (...) {
		std::cerr << "Error: main_parsing" << std::endl;
	}
	
//	LAN	all_servers(all_config);
//	all_servers.run();

	Network	serv(all_config[0], 0);		//not
	serv.run();							//amymore

	return (0);
}
