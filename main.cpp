#include "includes/Webserv_Includes.hpp"
#include "srcs/parse_config_file/Server.hpp"

int	main(int ac, char **av)
{

	if (ac != 2)
		Ft_error err("Bad arguments");

	std::cout << "🧚‍ Main_parsing launch" << std::endl;
	//Parsing deracineur de bitume
	std::vector<Server> all_server;
	try {
		main_parsing(av, all_server);
	}
	catch (...) {
		std::cerr << "Error: main_parsing" << std::endl;
	}
	// -> all_server full parsing config

	// How to use :
	Server *s1;
	s1 = &all_server[0];
	std::cout <<Y<<  "s1->getNameServer() = '" << s1->getNameServer()<<"'" <<RE<<std::endl;
	std::cout <<Y<<  "s1->getIpServer(); = '" << s1->getIpServer()<<"'" <<RE<<std::endl;
	std::cout <<Y<<  "s1->getPortServer(); = '" << s1->getPortServer()<<"'" <<RE<<std::endl;

	s1->getInLocationValue("/methods", "root");
	std::cout <<Y<<  "s1->getKeyTemp((); = '" << s1->getKeyTemp()<<"'" <<RE<<std::endl;
	std::cout <<Y<<  "s1->getValueTemp((); = '" << s1->getValueTemp()<<"'" <<RE<<std::endl;

	s1->getAllLocations();
	s1->getAllServerConfig();



	Network	serv(8080);
	serv.run();

	// start test // marco t bô <3
    Response r1;

	r1.set_content("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>404</title></head><body><h2>Error 404: not found</h2></body></html>", "text/html");

	std::cout << r1.send(0) << std::endl << r1 << std::endl;
	//end test // non g menti mdr
  return (0);
}
