#include "../../includes/Ft_error.hpp"

Ft_error::Ft_error(void)
{
	//std::cout << "Default constructor called\n";
	return ;
}

Ft_error::Ft_error(std::string str)
{
	//std::cout << "Parametric constructor called\n";
	std::cout << "Error: ";
	if (str.empty())
		std::cout << "fatal\n";
	else
		std::cout << str << std::endl;
	std::exit(1);
	return ;
}

Ft_error::Ft_error(Ft_error const &src)
{
	*this = src;
	//std::cout << "Copy constructor called\n";
	return ;
}

Ft_error::~Ft_error(void)
{
	//std::cout << "Destructor called\n";
	return ;
}

Ft_error	&Ft_error::operator=(Ft_error const &rhs)
{
	(void)rhs;

	//std::cout << "Copy assignment operator called\n";
	return (*this);
}
