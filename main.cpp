#include "includes/Webserv_Includes.hpp"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 2)
		Ft_error err("Bad arguments");
    
	Network	serv(8080);
	serv.run();

	// start test // marco t bô <3
    Response r1;

	r1.set_content("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><title>404</title></head><body><h2>Error 404: not found</h2></body></html>", "text/html");

	std::cout << r1.send(0) << std::endl << r1 << std::endl;
	//end test // non g menti mdr
  return (0);
}
