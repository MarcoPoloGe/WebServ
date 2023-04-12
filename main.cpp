#include "includes/Webserv_Includes.hpp"
#include "srcs/parse_config_file/Server.hpp"
#include "srcs/parse_config_file/Types.hpp"

int	main(int ac, char **av)
{

	if (ac != 2)
		Ft_error err("Bad arguments");

	/* Start Parsing deracineur de bitume */
	std::cout << "🧚‍ Main_parsing launch" << std::endl;
	std::vector<Server> all_server;
	Types t;
	try {
		main_parsing(av, all_server, t);
	}
	catch (...) {
		std::cerr << "Error: main_parsing" << std::endl;
	}
	// -> all_server full parsing config

	// How to use :
//	Server *s0;
//	s0 = &all_server[0];
//	std::cout <<Y<<  "s0->getNameServer() = '" << s0->getNameServer()<<"'" <<RE<<std::endl;
//	std::cout <<Y<<  "s0->getIpServer(); = '" << s0->getIpServer()<<"'" <<RE<<std::endl;
//	std::cout <<Y<<  "s0->getPortServer(); = '" << s0->getPortServer()<<"'" <<RE<<std::endl;
//
//	s0->getInLocationValue("/methods", "root");
//	std::cout <<Y<<  "s0->getKeyTemp((); = '" << s0->getKeyTemp()<<"'" <<RE<<std::endl;
//	std::cout <<Y<<  "s0->getValueTemp((); = '" << s0->getValueTemp()<<"'" <<RE<<std::endl;
//
//	s0->printAllContentsLocation("/methods");
//
//	s0->printAllLocation();
	std::cout << t.getPathFormat("csds") << std::endl;

//	s1->getAllLocations();
//	s1->getAllServerConfig();

	/* End Parsing deracineur de bitume */


	Network	serv(8080);
	serv.run();

	// start test // marco t bô <3
    Response r1;

	r1.set_content("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>404</title></head><body><h2>Error 404: not found</h2></body></html>", "text/html");

	std::cout << r1.send(0) << std::endl << r1 << std::endl;
	//end test // non g menti mdr
  return (0);
}
