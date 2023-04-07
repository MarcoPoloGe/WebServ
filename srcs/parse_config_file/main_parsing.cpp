
#include "Server.hpp"

//
void Read_and_Stock_FileName(
		const std::string& fileName,
		std::vector<std::string> &stock )
{
	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	// Read line by line
	while (std::getline(file, line))
		stock.push_back(line);
	file.close();
}

//
void printVector(std::vector<std::string> &x )
{
	for (std::vector<std::string>::iterator it = x.begin(); it != x.end(); it++)
		std::cout << *it << std::endl;
}

//
void getOnlyChar(std::string &s)
{
	s.erase(std::remove(s.begin(), s.end(), '\t'), s.end());
	s.erase(std::remove(s.begin(), s.end(), ' '), s.end());
	s.erase(std::remove(s.begin(), s.end(), '\n'), s.end());
}

// Insert in a map, an input with "=" as delimiter. ex: key= "location" | value="\methods"
void insertMap_split_by_Delimiter(
		std::map<std::string,
		std::string> &map,
		std::string input,
		std::string delimiter)
{
	unsigned long pos;
	if((pos = input.find(delimiter)) != std::string::npos)
	{
		std::string key = input.substr(0, pos);
		getOnlyChar(key);
		std::string value = input.substr(pos + 1, input.length());
		getOnlyChar(value);
		map.insert(std::pair<std::string, std::string>(key, value));
	}
	else
		std::cerr<< "Delimiter '" << delimiter << "' not found in " << input << std::endl;
}

// Set _locs :  a location
std::vector<std::string>::iterator grabLocation (
		std::vector<std::string>::iterator 	it,
		std::vector<std::string>::iterator 	last_bracket,
		Server &s)
{
	std::vector<std::string>::iterator 	first_bracket;
	std::map<std::string, std::string> loc_config;

	//Stock "location=(...) in a vector"
	if ((*it).find("location") != std::string::npos)
		insertMap_split_by_Delimiter(loc_config, *it, "=");

	while(it != last_bracket)
	{
		if (((*it).find("{"))!= std::string::npos)
		{
			it++;
			while (!(((*it).find("}")) != std::string::npos))
			{
				insertMap_split_by_Delimiter(loc_config, *it, "=");
				it++;
			}
			s.getAllLocations().push_back(loc_config);
			return (it);
		}
		it++;
	}
	if (it == last_bracket) {
		std::cerr<<R<< "Last '}' in location not found in config file" << std::endl;
	}
	return(it);
}
std::string AfterEqual (std::string input) {
	return (input.substr((input.find("="))+1, input.length()));
}
Server &setUpServer(
		std::vector<std::string>::iterator 	first_bracket,
		std::vector<std::string>::iterator 	last_bracket,
		Server &s)
{
	std::vector<std::string> 				in;
	std::vector<std::string> 				server_config;
	std::vector<std::vector<std::string> > 	locations;
	std::vector<std::string> 				loc;
	std::string								path;

	while (first_bracket != last_bracket)
	{
		if (((*first_bracket).find("location"))!= std::string::npos)
		{
			first_bracket = grabLocation(first_bracket, last_bracket, s);
			if(((*first_bracket).find("}"))!= std::string::npos)
				first_bracket++;
			continue;
		}
		if (((*first_bracket).find("name="))!= std::string::npos)
			s.setNameServer(AfterEqual(*first_bracket));
		if (((*first_bracket).find("ip="))!= std::string::npos)
			s.setIpServer(AfterEqual(*first_bracket));
		if (((*first_bracket).find("port="))!= std::string::npos)
			s.setPortServer(AfterEqual(*first_bracket));


		in.push_back(*first_bracket);
		first_bracket++;
	}
	return(s);
}


bool serverConfig(std::vector<std::string> &stock,
						std::vector<std::string>::iterator it,
						std::vector<Server> &all_server)
{
	std::vector<std::string>::iterator 					first_bracket;
	std::vector<std::string>::iterator 					last_bracket;

	std::vector<Server>::iterator 						vit;

	int get_in = 0;
	int count = 0;

	for(it = stock.begin(); it != stock.end(); it++)
	{
		if (((*it).find("{"))!= std::string::npos) {
			count += 1;
			if (count == 1) {
				first_bracket = it;
			}
		}
		else if (((*it).find("}")) != std::string::npos)
		{
			if (count == 1) {
				last_bracket = it + 1;
				get_in += 1;
			}
			count -= 1;
		}
		if (get_in == 1 && count == 0)
		{
			Server s;
			s.setRawfile(first_bracket, last_bracket);
			setUpServer(first_bracket, last_bracket, s);
			all_server.push_back(s);
			get_in = 0;
		}
	}
	return (true);
}

void main_parsing(char **av, std::vector<Server> &all_server) {

	std::string fileName = av[1];
	std::vector<std::string> rawfile;
	std::cout <<W<< "Parsing config file" <<RE<< std::endl;

	std::string line;
	std::ifstream file;

	// Try open file
	file.open(fileName.c_str());
	if (!file.is_open() || file.bad())
		throw std::bad_exception();

	// Read line by line
	while (std::getline(file, line))
		rawfile.push_back(line);
	file.close();

	// Set up Servers from config.conf file
	serverConfig(rawfile, rawfile.begin(), all_server);
}
