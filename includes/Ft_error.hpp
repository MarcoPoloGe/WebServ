#ifndef FT_ERROR_HPP
# define FT_ERROR_HPP

# include <iostream>
# include <string>
# include <cstdlib>
# include <stdexcept>

class Ft_error
{

public:
	Ft_error(std::string str);
	~Ft_error(void);

private:
	Ft_error(void);
	Ft_error(Ft_error const &src);

	Ft_error	&operator=(Ft_error const &rhs);

};

#endif
