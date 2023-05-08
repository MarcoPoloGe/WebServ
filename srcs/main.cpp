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
	catch (std::exception &e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return (1);
	}

	LAN	all_servers(all_config);
	all_servers.runAll();

//	Network	serv(all_config[0], 0);		//not
//	serv.run();							//amymore

//	CGI test;
//	std::cout << "wesh" << std::endl;
//	test.execute();
	return (0);
}
