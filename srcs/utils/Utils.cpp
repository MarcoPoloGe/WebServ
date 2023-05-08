#include "../../includes/Webserv_Includes.hpp"

std::string ft_read_file(std::string file_name)
{
	std::ifstream html_file;
	std::string content;
	std::string line;

	html_file.open(file_name.c_str(), std::ios::in | std::ios::out | std::ios::binary);
	if(html_file.is_open())
	{
		std::cout <<G<< "file read OK  : " << file_name << "\n" <<RE;
	} else
	{
		std::cout <<R<< "file read FAIL: " << file_name << "\n" <<RE;
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

std::string	ft_get_extension(std::string str)
{
	std::size_t last_point = str.rfind(".");

	if (last_point == std::string::npos)
	{
		std::cout << "No particular extension in string [" << str << "]\n";
		return ("");
	}
	else
		return ( str.substr(last_point) );
}

std::string ft_remove_nonprintable(std::string str)
{
	std::string ret;
	int i = 0;
	int size = 0;

	for (int j = 0; str[j] != 0; j++)
	{
		if (31 < str[j] && str[j] < 127)
			size++;
	}
	ret.resize(size);
	for (int it = 0; str[it] != 0; it++)
	{
		if (31 < str[it] && str[it] < 127)
			ret[i++] = str[it];
	}
	ret[i] = 0;
	return (ret);
}
