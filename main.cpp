#include "includes/Webserv_Includes.hpp"

int	main(int ac, char **av)
{
	(void)av;
	if (ac != 2)
		Ft_error err("Bad arguments");
	Network	serv(8080);
	serv.run();
	return (0);
}
