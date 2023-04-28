#include "../../includes/Webserv_Includes.hpp"

std::string ft_read_file(std::string file_name)
{
	std::ifstream html_file(file_name.c_str());
	std::string content;
	std::string line;

	if(html_file.is_open())
	{
		std::cout << "file read OK  : " << file_name << "\n";
	} else
	{
		std::cout << "file read FAIL: " << file_name << "\n";
	}
	while (std::getline(html_file, line))
	{
		content += line + '\n';
	}
	return (content);
}

unsigned short	ft_bswap16(unsigned short x)
{
	x = x >> 8 | x << 8;
	return (x);
}

unsigned int	ft_bswap32(unsigned int x)
{
	x = (x & BYTE_0) >> 24 | (x & BYTE_3) << 24 |
		(x & BYTE_1) >> 8  | (x & BYTE_2) << 8;
	return (x);
}

unsigned short	ft_htons(unsigned short s)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (s);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft_bswap16(s));
	return (0);
}

unsigned int	ft_htonl(unsigned int l)
{
	if (BYTE_ORDER == BIG_ENDIAN)
		return (l);
	else if (BYTE_ORDER == LITTLE_ENDIAN)
		return (ft_bswap32(l));
	return (0);
}

void	setnonblocking(int sock)
{
	if (fcntl(sock, F_SETFL, O_NONBLOCK) < 0)
		Ft_error	err("fcntl");

	return ;
}
