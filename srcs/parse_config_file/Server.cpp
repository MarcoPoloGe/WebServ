//
// Created by Lowell Zima on 3/23/23.
//

#include "Server.hpp"
#include "Parser.hpp"
#include <vector>

/**********************************************************************************************************************/
/***************************                       Con/Destructors	           		           ************************/
/**********************************************************************************************************************/


Server::Server() {
	std::cout <<W<< "++server" <<RE<< std::endl;
	std::vector<std::string> v;
}

Server::~Server() {
	std::cout <<W<< "--server" <<RE<< std::endl;
}

void Server::setPortServer(std::string 	port) {
	this->_port = port;
};


/**********************************************************************************************************************/
/***************************                       Utils		            		           ************************/
/**********************************************************************************************************************/


std::string	Server::searchValue (
		std::string key,
		std::vector<std::string> vect_source)
{
	std::vector<std::string>::iterator start = vect_source.begin();
	std::vector<std::string>::iterator end = vect_source.end();
	while(start != end) {
		if (((*start).find(key))!= std::string::npos)
			return((*start).substr(((*start).find("=")) + 1, (*start).length()));
		start++;
	}
	std::cerr <<R<< "searchValue: Keyword " << key << "not found in source " <<RE<< std::endl;
	return(NULL);
}

std::string &Server::returnAfterEqual (
		std::string key,
		std::string source)
{
	std::string ret;
	if ((source.find(key))!= std::string::npos)
		{
			ret = source.substr((source.find("=")) + 1, source.size());
			return(ret);
		}
	std::cerr <<R<< "returnAfterEqual: Keyword " << key << "not found in source " <<RE<< std::endl;
	return(ret);
}


std::vector<std::string> &Server::getSpecificLoc (
		std::string way,
		Server s)
{
	std::vector<std::vector<std::string> >::iterator start = s.getAllLocations().begin();
	std::vector<std::vector<std::string> >::iterator end = s.getAllLocations().end();
	while(start != end)
	{
		std::vector<std::string>::iterator path = s.getAllLocations().at(0).begin();
		if (((*path)).find(way) != std::string::npos)
			return(*start);
		start++;
	}
	return(*start);
}
